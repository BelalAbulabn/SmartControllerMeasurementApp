
/**
 * @file    control.c
 * @authors Belal Abulabn
 * @version 1.1
 * @date    2021-10-14
 *
 *
 *
 *Die spätere Aktor-Regelung sollte in control.c implementiert werden, da dort bereits das Auslesen der ADC-Werte
 *und die controlLoop implementiert sind. Funktionen zum Auslesen der Programmeingaben über RS-232 sind
 * in dieser Datei konfiguriert. Des Weiteren befinden sich neben der ADC-Genauigkeitskorrektur Funktionen zu wichtigen Statusausgaben,
 *ADCMessungen und LED-Ansteuerungen in dieser Datei. Die Änderung der ADC-, DAC- und controlLoop-Frequenz muss durch Anpassen des Prescaler-
 *und Period-Wertes innerhalb der Timer-Initialisierung vorgenommen werden

 */



/* ===========================================================================
 * Includes
 * ===========================================================================*/
#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_dac.h"
#include "config.h"
#include "control.h"
#include "gpio.h"
#include "math.h"
#include "pwm.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_flash.h"
#include "stdlib.h"
#include "inttypes.h"

/**
 * @{
 *
 */

/* ===========================================================================
 * Local Defines
 * ===========================================================================*/
#define buffsize_adc_cache 3		//e.g.: 21 = 3 ADCs x 7 ADC Channels
#define buffsize_adc_cache2 30000		//e.g.: 21 = 3 ADCs x 7 ADC Channels
#define RXBUFFERSIZE 60

/* ===========================================================================
 * Local Macros
 * ===========================================================================*/

/* ===========================================================================
 * Local Type-Definitions
 * ===========================================================================*/

/* ===========================================================================
 * Local Constants
 * ===========================================================================*/

uint32_t led_toggle = 0;

// RS232
volatile u8 RxBuffer[RXBUFFERSIZE];
volatile u8 RxCounter=0;
volatile u8 commandReceived=0;
// calculations
volatile float VDDA = 3.3163;		//todo calibration: Voltage level of STM32F405: measured value
volatile float VDDA_Analog_IN = 3.15;
/* todo calibration: Voltage level of Analog_INX on Testpoint TP_A1 when applied with +10 V
 * -> corrects summing amplifier safety boundaries of 0.15 V on upper and lower end
 */
//todo set the Hall voltage correction value (measured to get 0 V Hall voltage when 0 V are applied)
volatile float Hall_offset_value = -2045.968;		// = 1.032 - 2048 + 1

float VDDA_Analog_IN_factor = 0;	// stores the value to correct the Analog_INX input value with regards to summing amplifier, processed in main()
float two_point_five_volts_VREF_measured = 0; // measured value of 2.5V-VREF is: 2.50063 V which must be saved in int from 0 to 4095 with regards to VDDA, processed in main()
float to_volt_var = 0;
float to_volt_var_adc123 = 0;
float vrefint_volts_summ = 0;
float vrefint_volts_summ_global = 0;




uint16_t value_cache3 = 0;


char Duty_Float[30];
char PWM_Mode[30];

// Readoutbuffer
volatile uint16_t ADCbuf1_cache[buffsize_adc_cache] = {0};
volatile uint32_t ADCbuf1_cache2[buffsize_adc_cache2] = {0};



/* ===========================================================================
 * Local Variables
 * ===========================================================================*/

/* __________Clock Speeds_____________
 * Systemclock, Ethernet PTP, HCLK to AHB bus, core, memory and DMA, Cortex System Timer, FCLK Cortex clock, APB2 timer: 168 MHz
 * APB1 Timer, APB2 peripheral clocks: 84 MHz
 * APB1 peripheral clocks: 42 MHz
 */


/*============================================================================*/
/**
 * @brief      Initialization function for the SmartControl project.
 * @details    Inits the hardware step by step (RS232/ USART1 output first) and
 * 			   sets global variables for ADC readout and calculation.
 */

void init_SmartControl (void){

	usartConfig();
	printf("USART_Configuration done\r");
	GPIO_Configuration();
	printf("GPIO_Configuration done\r");
	NVIC_USART_Configuration();  			// for RS232 on USART1
	NVIC_DMA_Configuration();				// for ADC flag readout, when a new ADC value is available
	NVIC_controlLoop_Configuration(); 		// for controlLoop trigger
	printf("NVIC_Configuration done\r");
	Delay(10000);
	printf("SystemInit_Configuration done\r");
	DMA_Configuration();
	printf("DMA_Configuration done\r");
	ADC_Configuration_Three_Channels();
	printf("ADC_Configuration done\r");
	Config_ADCtimer_init();
	printf("ADCtimer_Configuration done\r");

	SetControlLoopCallback(dummyLoop);
	Config_controlLooptimer_init();
	printf("controlLooptimer_Configuration done\r");
    PWM_init();
	printf("PWM_Configuration done\r");
	Delay(100000);
	printf("STM32F4: general initialization finished\r");
	EN_PWR();
	printf("EN_PWR for 9 V and 21.5 V done\r");
	printf("press 'h' for help (terminated each command with a carriage return)\r");
    EnableTimerInterrupt();

	Delay(100000);

	//_____ calculate variables _____
	//set the summing amplifier correction value of all analog inputs.
	VDDA_Analog_IN_factor = (float) (2 - (VDDA_Analog_IN / VDDA));
	//set 2.5V-VREF in int
	two_point_five_volts_VREF_measured = (float) ((2.50063 * 4095) / VDDA); //somewhere around 3088
	to_volt_var = VDDA / 4095;
	to_volt_var_adc123 = 10 / (2047.5 / VDDA_Analog_IN_factor);
	FLASH_Unlock();

}

/*============================================================================*/
/**
 * @brief      Main Control Loop of the programm. Add your functions here.
 * @details    Call this function via RS232 / USART1. Type 'h' for more commands.
 * 			   Comment functions that you don't need for improving performance.
 *
 */
void controlLoop(void){
	ADC_Configuration_Two_Channles_Coil_Current();
	DMA_Configuration_control_loop();
	/**	____________________________________________________________________________________
		 *
		 *									ADD YOUR OWN FUNCTIONS HERE
		 *
		 *
		 *
		 *
		 *		Coil1:
		 *			TIM1->CCR1 = (uint32_t)%Frequency;
		 *			TIM1->CCR2 = (uint32_t)%Frequency;
		 *		Coil2:
		 *			TIM1->CCR3 = (uint32_t)%Frequency;
		 *			TIM8->CCR1 = (uint32_t)%Frequency;
		 *
		 *		Or set Duty Cycle manually with command "#X duty" (X= 1/2/3)
		 *		Enter "h" in HTerm for additional commands.
		 *
		 *	____________________________________________________________________________________*/





}


/*============================================================================*/
/**
* @brief      This function does nothing except some LED flickering.
* @details    This function is slow (Interesting LED inside)!
*/
void dummyLoop(void){
	//Demo for LEDs
	interesting_led();
	//led();
}


// This functions send text or char array passed as argument over USART
void USART_SendText(volatile char *s)
{
  while(*s){
    // wait until data register is empty
    while( !USART_GetFlagStatus(USART1, USART_FLAG_TXE) );
    USART_SendData(USART1, *s);
    s++;
  }
}

void USART_SendFloat(float k)
{
    // wait until data register is empty
    while( !USART_GetFlagStatus(USART1, USART_FLAG_TXE) );
    printf("  %f,",k);
}
// This function sends numbers up to 32bit over USART
void USART_SendNumber(uint32_t x)
{
  char value[10]; //a temp array to hold results of conversion
  int i = 0; //loop index

  do
  {
    value[i++] = (char)(x % 10) + '0'; //convert integer to character
    x /= 10;
  } while(x);

  while(i) //send data
  {
    //USART_SendNumber8b(USARTx, value[--i]);
    while( !USART_GetFlagStatus(USART1, USART_FLAG_TXE) );
    USART_SendData(USART1, value[--i]);
  }
}





/*============================================================================*/
/**
 * @brief      Evaluates user input commands via RS232 connection.
 * @details    To see the full list of available commands put in "h" in your RS232 console.
 * 			   Each user input must have a carriage return (CR).
 */
void usartReceived(void){

	char fullCommand[50]="";
	char command[30]="";	//command between #X and Y/S
	char valstring[30]="";	//S
	int args=0;
	int id=-1;
	int x=-1;
	int y=-1;
	int z=-1;
	if (RxCounter == 0) {
		printf("Unknown Command\r");
	}
	else if (RxCounter == 1)
	{

		if (RxBuffer[0]=='s')
		{
			id = 0;
			Init_PWM_Operation_Mode((int)id);		// Disable PWM
			SetControlLoopCallback(dummyLoop);	// Sets dummyLoop
		}

		else
		{
			printf("Unknown Command\r");
		}
	}
	else if (RxCounter > 1)
	{
		strncpy(fullCommand, (char*)RxBuffer, RxCounter);
		fullCommand[RxCounter]='\0';
		printf("%s\r",fullCommand);


		args = sscanf(fullCommand, "%s", command);

		if (strcmp(command, "ctrl")==0)
		{
			SetControlLoopCallback(dummyLoop);		// Sets dummyLoop
			SetControlLoopCallback(controlLoop);	// Sets controlLoop
		}

		else if (strcmp(command, "en")==0)
		{
			EN_PWR();								// Enables 21.5 V and 9 V voltage supply
		}
		else if (strcmp(command, "dis")==0)
		{
			Disable_PWR();							// Disables 21.5 V and 9 V voltage supply
		}

		else{

		args = sscanf(fullCommand, "%d/%d/%d/%d",&z, &id, &x, &y);

		}
		if (RxCounter >= 2){

			if (args>=2){
				if(z==3)
				{
					ADC_Sampling((int)id,(int)x);
				}

				// #1 - #4 Commands for single testStand
				else if ((id>=1) & (id<=3)){
					if(z==1){
						Init_PWM_Operation_Halb_Bridig((int)id,(int)x,(int)y);
					}
					if(z==2){
						Init_PWM_Operation_Full_Bridig((int)id,(int)x,(int)y);
						}
					if(z==4){
						Sprungsignal((int)id,(int)x,(int)y);
							}

					else printf("Unknown Command\r");
				}				else printf("Unknown Command\r");
			}
		}
		else
		{
			printf("Unknown Command\r");
		}
	}
	RxCounter = 0;
	commandReceived = 0;
}


float Return_Duty_Float (void){
	float string = 0;
	string = strtof(Duty_Float, NULL);	//convert user duty cycle string into float
	return string;
}


/*============================================================================*/
/**
 * @brief      Return function for commandReceived.
 * @details    returns the u8 commandReceived variable for RS232 data transfer status
 * 			   readout on USART1.
 */
u8 Return_commandReceived (void){
	return commandReceived;
}


/*============================================================================*/
/**
 * @brief      Interrupt Handler for USART1, is executed when a char is received
 * @details    Adds the received char to RxBuffer, sets commandReceived to 1 when a carriage return is received
 */
void USART1_IRQHandler(void) {
	u8 tmp;
	tmp=USART_ReceiveData(USART1);	// Read one byte from the receive data register
	USART_ClearITPendingBit(USART1, USART_IT_RXNE); // Clear the USART1 Receive interrupt

	if (commandReceived == 0) {
		if (tmp=='\r') {
			commandReceived=1;
		}
		else {
			if (RxCounter >= RXBUFFERSIZE-1) RxCounter = 0;
			RxBuffer[RxCounter++] = tmp;
		}
	}
}


/*============================================================================*/
/**
 * @brief      Function that sets both DAC Outputs.
 * @details    Choose data-align: DAC_Align_12b_R and the two DAC values.
 */
void DAC_Set_Output(uint16_t DAC2_Value, uint16_t DAC1_Value){
	//DAC_SetDualChannelData(DAC_Align_12b_R, DAC2_Value, DAC1_Value);	//set desired DAC values to both DACs
	DAC_SetDualChannelData(DAC_Align_12b_R, (uint16_t)DAC2_Value, (uint16_t)DAC1_Value);
}


/*============================================================================*/
/**
 * @brief      Output signal to enable and start 9 V and 21.5 V power supplies.
 * @details    Disable to save power or completely shut down the PWM-Module/ Hall-Power Supply.
 * 			   Enable after initial start of the STM32F405. Port: PC13 with pulldown resistor.
 */
void EN_PWR(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	printf("done\r");


	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}


/*============================================================================*/
/**
 * @brief      Output signal to disable 9 V and 21.5 V power supplies.
 * @details    Disable to save power or completely shut down the PWM-Module/ Hall-Power Supply.
 * 			   Enable after initial start of the STM32F405. Port: PC13 with pulldown resistor.
 */
void Disable_PWR(void){
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}


/*============================================================================*/
/**
 * @brief      Function that saves all ADC values in a cache.
 * @param 	   cachsize_s - unsigned int defines size of cache (0x0000 to (0xFFFF / buffsize_adc))
 * @details    This function uses an array as a readout cache in order to prevent an
 * 			   overwrite of ADC values from the DMA handler. It can store up to 0xFFFF / buffsize_adc
 * 			   values for buffering ADC readout.
 */


void ADC_Save_Values_In_Cache(uint32_t cachsize_s){
	uint32_t g = 0;
	uint32_t p = 0;
	printf("\r---please wait---\r\n");
	// Write values in array to save them before printf (printf is to slow; can't keep up with sending speed)


	//&& DMA_GetFlagStatus(DMA2_Stream0, DMA_FLAG_TCIF0)!= RESET
	while (g < cachsize_s){
		g++;
		if (Return_New_ADC_conversion_available() != 0) {
			for (p = 0; p < Return_buffsize_adc(); p++){
				*(ADCbuf1_cache2 + ((g * Return_buffsize_adc()) + p)) = Return_ADCbuf1(p);
			}
			// reset for new conversions
			Reset_New_ADC_conversion_available();
		}
		else {
			g--;
		}
	}
	printf("\r---finished---");
}



/*============================================================================*/
/**
 * @brief     Function that reads all ADC values in a cache by controling timer ADC DMA, ADC_Save_Values_In_Cache function for ADC and then to print them.
 *
 * @details    With samplingfrequency, the sampling frequency is set by Timer 3 ,this Timer is used with ADC_Save_Values_In_Cache funktion
 */
/*============================================================================*/
/**
 * @brief      Function that reads all ADC values in a cache.
 * @param 	   cachsize_r - unsigned int defines size of cache (0x0000 to (0xFFFF / buffsize_adc))
 * @details    This function uses an array as a readout cache in order to prevent an
 * 			   overwrite of ADC values from the DMA handler. It can store up to 0xFFFF / buffsize_adc
 * 			   values for buffering ADC readout.
 */

void ADC_Sampling (int Samplingfrequenz,int Numberofsamplis){
	TIM_DeInit(TIM3);
    TIM_TimeBaseInitTypeDef Timer_Struct;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    Timer_Struct.TIM_Period = Samplingfrequenz;

    Timer_Struct.TIM_Prescaler = 0; 		// 0
    Timer_Struct.TIM_CounterMode = TIM_CounterMode_Up;
    Timer_Struct.TIM_ClockDivision = TIM_CKD_DIV1;
    Timer_Struct.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIM3, &Timer_Struct);  //Enabling Timer 3
    TIM_SelectOutputTrigger(TIM3,TIM_TRGOSource_Update);
    TIM_Cmd(TIM3,ENABLE);

	uint32_t m = 0;
	uint32_t q = 0;
	uint16_t value_cache2 = 0;

	ADC_Save_Values_In_Cache(Numberofsamplis);			// Saves Samplies values and readouts.


	for (m = 0; m < Numberofsamplis; m++){

		for (q = 0; q < Return_buffsize_adc(); q++){
				value_cache2 = *(ADCbuf1_cache2 + ((m * Return_buffsize_adc()) + q));
					if (q == 0 ) {
						 printf("PA0->Analog_IN_1");printf("%f,", (10 * ((float)(value_cache2) - 2047.5) / (2047.5 / VDDA_Analog_IN_factor)));
									}
					if (q == 1){
						printf("PA1->Analog_IN_2");printf("%f,",(10 * ((float)(value_cache2) - 2047.5) / (2047.5 / VDDA_Analog_IN_factor)));
								}
					if (q == 2 ){
						printf("PA2->Analog_IN_3");printf("%f,\n",(10 * ((float)(value_cache2) - 2047.5) / (2047.5 / VDDA_Analog_IN_factor)));
								}

				}


			}
}

/*============================================================================*/
/**
 * @brief      Delay function to do nothing over a certain amount of time.
 * @param 	   nCount - unsigned int
 * @details    Keeps the controller busy for the time it counts up. Counts with SystemClock speed (168MHz).
 */
void Delay(__IO uint32_t nCount) {
while(nCount--) {
	}
}
/*============================================================================*/
/**
 * @brief      Delay function to do nothing over a certain amount of time.
 * @param 	   nCount - unsigned int
 * @details    Keeps the controller busy for the time it counts up. Counts with SystemClock speed (168MHz).
 */
void Delaysec(__IO uint32_t nCount) {
	nCount=nCount*672*10000;
	while(nCount--) {

	}
}
/*============================================================================*/
/**
 * @brief      Delay function to do nothing over a certain amount of time.
 * @param 	   nCount - unsigned int
 * @details    Keeps the controller busy for the time it counts up. Counts with SystemClock speed (168MHz).
 */
void Delaymlsec(__IO uint32_t nCount) {
	nCount=nCount*672*10;
	while(nCount--) {

	}
}
/*============================================================================*/
/**
 * @brief      Delay function to do nothing over a certain amount of time.
 * @param 	   nCount - unsigned int
 * @details    Keeps the controller busy for the time it counts up. Counts with SystemClock speed (168MHz).
 */
void Delayusec(__IO uint32_t nCount) {
	nCount=nCount*7;
	while(nCount--) {

	}
}



/*============================================================================*/
/**
 * @brief      Returns the desired PWM Mode for Coil_2
 * @details    Returns 0 for wrong input, 1 for one 3-phase actuator or 8 for two 2-phase actuators.
 */
float Return_PWM_Mode(void) {
	float string = 0;
	string = strtof(PWM_Mode, NULL);	//convert user PWM mode string into float
	if (string == 1.0 || string == 8.0){
		return string;
	} else return 0.0;
}


/*============================================================================*/
/**
 * @brief      Toggle function for GPIO Pins.
 * @param 	   GPIOx - defines the GPIO Port, GPIO_Pin - defines the special GPIO Pin on this port
 * @details    This function will set the opposite state of the desired GPIO pin.
 */
void togglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){

	  if (((GPIOx->ODR) & GPIO_Pin) != (uint32_t)Bit_RESET)
	  {
		  GPIOx->BSRRH = GPIO_Pin;
	  }
	  else
	  {
		  GPIOx->BSRRL = GPIO_Pin;
	  }
}


/*============================================================================*/
/**
 * @brief      LED Demonstration
 * @details    Call from mainLoo() for a demonstration of the 4 LEDs.
 * 			   Attention: You may not want to use this in performance tests.
 */
void interesting_led (void){


	if (led_toggle == 200000){
		GPIO_ToggleBits(GPIOD, GPIO_Pin_10);
		led_toggle = 0;
	}
	else if (led_toggle == 175000){
		GPIO_ToggleBits(GPIOD, GPIO_Pin_11);

	}
	else if (led_toggle == 150000){
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);

	}
	else if (led_toggle == 125000){
		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);

	}
	else if (led_toggle == 100000){
		GPIO_ToggleBits(GPIOD, GPIO_Pin_11);

	}
	else if (led_toggle == 97000){
		GPIO_ToggleBits(GPIOD, GPIO_Pin_10);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_11);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);

	}
	else if (led_toggle == 93000){
		GPIO_ToggleBits(GPIOD, GPIO_Pin_10);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_11);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);

	}
	else if (led_toggle == 80000){
		GPIO_ToggleBits(GPIOD, GPIO_Pin_10);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_11);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);

	}
	else if (led_toggle == 75000){
		GPIO_ToggleBits(GPIOD, GPIO_Pin_10);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_11);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);

	}
	else if (led_toggle == 70000){
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);

	}
	else if (led_toggle == 67500){
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);

	}
	else if (led_toggle == 65000){
		GPIO_ToggleBits(GPIOD, GPIO_Pin_11);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);

	}
	else if (led_toggle == 62500){
		GPIO_ToggleBits(GPIOD, GPIO_Pin_10);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_11);

	}
	else if (led_toggle == 60000){
		GPIO_ToggleBits(GPIOD, GPIO_Pin_10);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);

	}
	else if (led_toggle == 57500){
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);

	}
	else if (led_toggle == 55000){
		GPIO_ToggleBits(GPIOD, GPIO_Pin_11);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);

	}
	else if (led_toggle == 52500){
		GPIO_ToggleBits(GPIOD, GPIO_Pin_10);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_11);

	}
	else if (led_toggle == 50000){
		GPIO_ToggleBits(GPIOD, GPIO_Pin_10);

	}
	else if (led_toggle == 47000){
		GPIO_ToggleBits(GPIOD, GPIO_Pin_10);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_11);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);

	}
	else if (led_toggle == 33000){
		GPIO_ToggleBits(GPIOD, GPIO_Pin_10);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_11);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);

	}
	else if (led_toggle == 20000){
		GPIO_ToggleBits(GPIOD, GPIO_Pin_10);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_11);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);

	}
	else if (led_toggle == 10000){
		GPIO_ToggleBits(GPIOD, GPIO_Pin_10);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_11);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);

	}
	else {}
	led_toggle++;

}


