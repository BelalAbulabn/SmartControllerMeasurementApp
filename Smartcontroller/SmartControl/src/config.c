/*============================================================================*/
/**
 * @file    config.c
 * @authors Belal Abulabn
 * @version 1.1
 * @date    2021-10-14
*  In dieser Datei befinden sich Timer, Interrupt, USART, DMA, ADC und
* Initialisierungen.

 */
 /* ==========================================================================*/

/* ===========================================================================
 * Includes
 * ===========================================================================*/
#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_dac.h"
#include "control.h"
#include "config.h"
#include "gpio.h"
#include "pwm.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stdlib.h"
#include "inttypes.h"

/**
 * @{
 *
 */

/* ===========================================================================
 * Local Defines
 * ===========================================================================*/
#define buffsize_adc 21		 //e.g.: 21 = 3 ADCs x 7 ADC Channels
#define buffsize_dac 32		 // 16-bit x 64 / 2
#define buffsize_adc1 3      //3 ADC Channels
/* ===========================================================================
 * Local Macros
 * ===========================================================================*/

/* ===========================================================================
 * Local Type-Definitions
 * ===========================================================================*/

/* ===========================================================================
 * Local Constants
 * ===========================================================================*/
/*
adc buffer size = Anzahl_der_meisten_Kanäle_eines_ADC * Anzahl_der_ADCs
z.B.: ADC1 hat 4 Kanäle, ADC2 hat 1 Kanal und ADC3 hat 0 Kanäle: buffsize_adc ist 4*2=8
Die Auslesung für diesen Fall wird sein: ADC1_Kanal1, ADC2_Kanal1, ADC1_Kanal2, ADC2_Kanal2, ADC1_Kanal3, ..., ADC2_Kanal4
auch wenn ADC2 nur einen Kanal hat!
*/
volatile uint16_t ADCbuf1[buffsize_adc1] = {0};
volatile uint16_t ADCbuf2[buffsize_adc1] = {0};
volatile uint16_t DACbuf1[buffsize_dac] = {0};
static void (*which_controlLoop)(void);
uint8_t volatile new_adc_value = 0;
uint8_t volatile new_adc_value2 = 0;

/* ===========================================================================
 * Local Variables
 * ===========================================================================*/

/* __________Clock Speeds_____________
Systemtakt, Ethernet PTP, HCLK für AHB-Bus, Kern, Speicher und DMA, Cortex System Timer, FCLK Cortex-Takt, APB2-Timer: 168 MHz
APB1-Timer, APB2-Peripherie-Takte: 84 MHz
 APB1-Peripherie-Taktgeber: 42 MHz
 */



/*============================================================================*/
/*
@brief 		 Rückgabefunktion für den ADC-DMA-Puffer.
@param	     int adc_buf_position wählt die gewünschte Array-Position von ADCbuf1.
@details     z.B. adc buffer size = Nr._of_most_channels_of_any_ADC  Nr._of_ADCs
             z.B.: ADC1 hat 4 Kanäle, ADC2 hat 1 Kanal und ADC3 hat 0 Kanäle: buffsize_adc ist 4*2=8
             Die Auslesung für diesen Fall ist: ADC1_Kanal1, ADC2_Kanal1, ADC1_Kanal2, ADC2_Kanal2, ADC1_Kanal3, ..., ADC2_Kanal4
              auch wenn ADC2 nur einen Kanal hat!
 */
uint16_t Return_ADCbuf1 (int adc_buf_position){
	return ADCbuf1[adc_buf_position];
}


/*============================================================================*/
/**
 * @brief      Return function for the DAC DMA buffer.
 * @param	   int dac_buf_position selects the desired array position of DACbuf1.
 * @details    e.g. adc buffer size = Nr._of_most_channels_of_any_ADC * Nr._of_ADCs
 * 			   e.g.: ADC1 has 4 channels, ADC2 has 1 channel and ADC3 has 0 channels: buffsize_adc is 4*2=8
 *			   readout for this case will be: ADC1_channel1, ADC2_channel1, ADC1_channel2, ADC2_channel2, ADC1_channel3, ..., ADC2_channel4
 *  		   even if ADC2 only has one channel!
 */
uint16_t Return_DACbuf1 (int dac_buf_position){
	return DACbuf1[dac_buf_position];
}


/*============================================================================*/
/**
 * @brief      Return function for the size of buffsize_adc.
 * @details    Returns buffer size of buffsize_adc.
 */
uint16_t Return_buffsize_adc (void){
	return buffsize_adc1;
}


/*============================================================================*/
/**
 * @brief      USART configuration for RS232 on USART1 and port GPIOB.
 * @details    GPIOB: 	Pin10 as alternate function = transmit data (TX)
 * 						Pin11 as alternate function = receive data  (RX)
 * 						todo those pins are selected for test purposes
 * 						and need to be mapped on pins: PA9 (TX) and PA10 (RX)
 */
void usartConfig(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* Usart1.Tx (PA9, yellow) und Rx (PA10, orange) Pins  */
	/* Configure USART1 Tx (PA9) and Rx (PA10) as alternate function push-pull */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	/* Configure USART1 Rx (PA10) as alternate function nopull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	/* Usart1 Config */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	/* Configure USART1 */
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);

    /* Buffer für Standardausgabe auf 0 sonst wird _write() von printf() nicht aufgerufen */
	setbuf(stdout, NULL);
    setbuf(stderr, NULL);
    setbuf(stdin, NULL);
}


/*============================================================================*/
/**
 * @brief      Interrupt configuration for RS232 on USART1.
 * @details    Called in main() on startup.
 */
void NVIC_USART_Configuration(void) {
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitTypeDef NVIC_InitStructure;

	//Enable the USARTx Interrupt: receive commands via usart
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}




/*============================================================================*/
/**
 * @brief      Timer 3 initialization for ADC DMA use
 * @details    max. 84 MHz ADC timer. Basic setup.
 */
void Config_ADCtimer_init(void)  //Regular ADC Timer
{
		TIM_DeInit(TIM3);
        TIM_TimeBaseInitTypeDef Timer_Struct;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
        Timer_Struct.TIM_Period = 1350;
        Timer_Struct.TIM_Prescaler = 0; 		// 0
        Timer_Struct.TIM_CounterMode = TIM_CounterMode_Up;
        Timer_Struct.TIM_ClockDivision = TIM_CKD_DIV1;
        Timer_Struct.TIM_RepetitionCounter = 0;

        TIM_TimeBaseInit(TIM3, &Timer_Struct);  //Enabling Timer 3
        TIM_SelectOutputTrigger(TIM3,TIM_TRGOSource_Update);
    	//TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);

        TIM_Cmd(TIM3,ENABLE);

       TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
}






void EnableTimerInterrupt()
{
    NVIC_InitTypeDef nvicStructure;
    nvicStructure.NVIC_IRQChannel = TIM5_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 1;
    nvicStructure.NVIC_IRQChannelSubPriority = 3;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);
}

/*============================================================================*/
/**
 * @brief      Timer 5 initialization for controlLoop interrupt.
 * @details    max. 84 MHz controlLoop timer. Basic setup.
 */
void Config_controlLooptimer_init(void)  //Regular ADC Timer
{

	TIM_DeInit(TIM5);
	TIM_TimeBaseInitTypeDef Timer_Struct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	Timer_Struct.TIM_Period = 1000;
	Timer_Struct.TIM_Prescaler = 0; 		// for 20 kHz: ((84 MHz / 20 kHz) / TIM_Period - 1) =  1 with TIM_Period=2100
	Timer_Struct.TIM_CounterMode = TIM_CounterMode_Up;
	Timer_Struct.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM5, &Timer_Struct);  //Enabling Timer 5
	TIM_SelectOutputTrigger(TIM5,TIM_TRGOSource_Update);
	TIM_ITConfig(TIM5, TIM_IT_CC1, ENABLE);

	TIM_Cmd(TIM5,ENABLE);
}


/*============================================================================*/
/**
 * @brief      Interrupt for DMA2_Stream0 (ADC9).
 * @details    Needed to read new ADC values when they are available.
 */
void NVIC_DMA_Configuration(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitTypeDef NVIC_InitStructure;
    /* Enable the DMA Stream IRQ Channel */
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


/*============================================================================*/
/**
 * @brief      Interrupt for TIM5_IRQn
 * @details    Needed to trigger controlLoop().
 */
void NVIC_controlLoop_Configuration(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitTypeDef NVIC_InitStructure;
    /* Enable the DMA Stream IRQ Channel */
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


/*============================================================================*/
/**
 * @brief      Interrupt Handler for DMA2_Stream0 (ADC)
 * @details    Waits for new transfer complete interrupt on DMA2_Stream0.
 */
void DMA2_Stream0_IRQHandler(void) {
	FlagStatus temp;
	temp = DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0);
	//temp = DMA_GetFlagStatus(DMA2_Stream0, DMA_FLAG_TCIF0);
	DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0); // Clears the DMA's transfer complete interrupt
	//DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0); // Clears the DMA's transfer complete interrupt

	if (temp != 0) {
		new_adc_value = 1;
		//new_adc_value2 = 1;

	//  GPIO_ToggleBits(GPIOC, GPIO_Pin_11);


	}
	else {
	}
}


/*============================================================================*/
/**
 * @brief      Interrupt Handler for TIM5 (controlLoop)
 * @details    Waits for TIM5 period.
 */

void TIM5_IRQHandler(void) {
	FlagStatus temp;
	temp = TIM_GetITStatus(TIM5, TIM_IT_CC1);
	//temp = DMA_GetFlagStatus(DMA2_Stream0, DMA_FLAG_TCIF0);
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1); // Clears the DMA's transfer complete interrupt
	//DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0); // Clears the DMA's transfer complete interrupt

	if (temp != 0) {
		which_controlLoop();
	  //  GPIO_ToggleBits(GPIOC, GPIO_Pin_11);

	}
	else {
	}
}

/*============================================================================*/
/**
 * @brief      Interrupt Handler for TIM3 (controlLoop)
 * @details    Waits for TIM3 period.
 */

void TIM3_IRQHandler(void) {
	FlagStatus temp;
	temp = TIM_GetITStatus(TIM3, TIM_IT_Update);
	//temp = DMA_GetFlagStatus(DMA2_Stream0, DMA_FLAG_TCIF0);
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // Clears the DMA's transfer complete interrupt
	//DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0); // Clears the DMA's transfer complete interrupt

	if (temp != 0) {
	   //GPIO_ToggleBits(GPIOC, GPIO_Pin_11);


	}
	else {
	}
}




/*============================================================================*/
/**
 * @brief         Sets the callback function for the controlLoop timer interrupt.
 * @param         *CallbackFunction Pointer to a function like: void controlLoop(void)
 * 				  or void dummyLoop(void)
 */
void SetControlLoopCallback(void (*CallbackFunction)(void) )
{
    which_controlLoop = CallbackFunction;
}


/*============================================================================*/
/**
 * @brief      Return function if new ADC values are available.
 * @details    Returns new_adc_value (new_adc_value = 1 means a new ADC readout is performed)
 */
uint8_t Return_New_ADC_conversion_available(void){
	return new_adc_value;
}


/*============================================================================*/
/**
 * @brief      Resets new_adc_value after a readout.
 * @details    Call after full readout is done.
 */
void Reset_New_ADC_conversion_available(void){
	new_adc_value = 0;
}


/*============================================================================*/
/**
 * @brief      Return function if new ADC values are available.
 * @details    Returns new_adc_value2 (new_adc_value2 = 1 means a new ADC readout is performed)
 */
uint8_t Return_New_ADC_conversion_available_2(void){
	return new_adc_value2;
}


/*============================================================================*/
/**
 * @brief      Resets new_adc_value2 after a readout.
 * @details    Call after full readout is done.
 */
void Reset_New_ADC_conversion_available_2(void){
	new_adc_value2 = 0;
}






/*============================================================================*/
/**
 * @brief      Analog to Digital Converter Setup (ADC), 1 ADC, 3 Channels, with DMA
 * @details    12 Bit resolution: 0 to 4095.
 * 			   -10 V to 10 V input range on Analog_IN1 (PA0), Analog_IN2 (PA1) and Analog_IN3 (PA2), (0.15 V to 3.15 V @ µC),
 * 			   		with low-pass-filter option, refer to datasheet
 *
 */

void ADC_Configuration_Three_Channels(void)
{
	ADC_DeInit();

	//RCC configuration is done in RCC_Configuration()
	//GPIO configuration is done in GPIO_Configuration()

	  /* ADC Common Init */
	  ADC_CommonInitTypeDef ADC_CommonInitStructure;
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);


	  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;	//use for only one ADC
	  //ADC_CommonInitStructure.ADC_Mode = ADC_TripleMode_RegSimult;	//use for three ADCs
	  //ADC_CommonInitStructure.ADC_Mode = ADC_DualMode_RegSimult;  //use for two ADCs
	  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;	// reads out ADC1-Ch1, ADC2-Ch1, ADC3-Ch1, ADC1-Ch2, ADC2-Ch2, ADC3-Ch2, ADC1-Ch3, ...
	  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	  ADC_CommonInit(&ADC_CommonInitStructure);

	  //ADC_DeInit();
	  ADC_TempSensorVrefintCmd(ENABLE);		//important to enable the vrefint and temp.-sensor ADC
	  ADC_VBATCmd(ENABLE);					//important to enable the vbat-sensor ADC

	  /* ADC1 Init */
	  ADC_InitTypeDef ADC_InitStructure;
	  ADC_InitStructure.ADC_ScanConvMode = ENABLE; // enable: Use of more than 1 Channel, otherwise disable!
	  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // no continuous conversion: trigger
	  ADC_InitStructure.ADC_NbrOfConversion = 3; //Number of channels
	  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
	  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
	  //ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
	  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	  ADC_Init(ADC1, &ADC_InitStructure);




	  //Setup of the channels
	  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_3Cycles); //Analog_IN_1 PA0;

	  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_3Cycles); //Analog_IN_2	PA1
	  ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_3Cycles); //Analog_IN_3	PA2

	  // Single Mode for desired ADC
	  ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);
	  // Multi ADC Mode
	 // ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);

	  /* Enable DMA */
	  ADC_DMACmd(ADC1, ENABLE);

	  /* Enable ADC1 */
	  ADC_Cmd(ADC1, ENABLE);



}
/*============================================================================*/
/**
 * @brief      Analog to Digital Converter Setup (ADC), 1 ADC, 3 Channels, with DMA
 * @details    12 Bit resolution: 0 to 4095.
 * 			   -10 V to 10 V input range on Analog_IN1 (PA0), Analog_IN2 (PA1) and Analog_IN3 (PA2), (0.15 V to 3.15 V @ µC),
 * 			   		with low-pass-filter option, refer to datasheet
 *
 */

void ADC_Configuration_Two_Channles_Coil_Current(void)
{
	ADC_DeInit();

	//RCC configuration is done in RCC_Configuration()
	//GPIO configuration is done in GPIO_Configuration()

	  /* ADC Common Init */
	  ADC_CommonInitTypeDef ADC_CommonInitStructure;
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);


	  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;	//use for only one ADC
	  //ADC_CommonInitStructure.ADC_Mode = ADC_TripleMode_RegSimult;	//use for three ADCs
	  //ADC_CommonInitStructure.ADC_Mode = ADC_DualMode_RegSimult;  //use for two ADCs
	  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;	// reads out ADC2-Ch1, ADC2-Ch2

	  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	  ADC_CommonInit(&ADC_CommonInitStructure);

	  //ADC_DeInit();
	  ADC_TempSensorVrefintCmd(ENABLE);		//important to enable the vrefint and temp.-sensor ADC
	  ADC_VBATCmd(ENABLE);					//important to enable the vbat-sensor ADC

	  /* ADC1 Init */
	  ADC_InitTypeDef ADC_InitStructure;
	  ADC_InitStructure.ADC_ScanConvMode = ENABLE; // enable: Use of more than 1 Channel, otherwise disable!
	  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // no continuous conversion: trigger
	  ADC_InitStructure.ADC_NbrOfConversion = 2; //Number of channels
	  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T5_CC2;
	  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
	  //ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
	  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	  ADC_Init(ADC1, &ADC_InitStructure);




	  //Setup of the channels
	  ADC_RegularChannelConfig(ADC2, ADC_Channel_10, 1, ADC_SampleTime_3Cycles); //Coil_Current_1 PC0 (note: stm discovery-board pin already used (->not accurate))
	  ADC_RegularChannelConfig(ADC2, ADC_Channel_11, 2, ADC_SampleTime_3Cycles); //Coil_Current_2 PC1


	  // Single Mode for desired ADC
	  ADC_DMARequestAfterLastTransferCmd(ADC2,ENABLE);
	  // Multi ADC Mode
	 // ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);

	  /* Enable DMA */
	  ADC_DMACmd(ADC2, ENABLE);

	  /* Enable ADC1 */
	  ADC_Cmd(ADC2, ENABLE);



}




/*============================================================================*/
/**
 * @brief      Configuration of DMA for ADC  data transfer
 * @details    DMA for ADC on: DMA2_Stream0 and DMA for
 * 			   Size of buffsize_adc  is configured to the actual setup.
 * 			   e.g. ADC buffer size = Nr._of_most_channels_of_any_ADC * Nr._of_ADCs
 *			   e.g.: ADC1 has 3 channels : buffsize_adc is 3
 *			   readout for this case will be: ADC1_channel1, ADC1_channel2, ADC1_channel3..
 */
void DMA_Configuration(void){
//	DMA_DeInit(DMA2_Stream0);  								//Set DMA registers to default values
	DMA_InitTypeDef  DMA_InitStructure;						//create DMA structure

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    //DMA for ADC
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(ADC1_BASE + 0x308);	//source ADC1's Data Register + Offset
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;			//set to channel 0
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;			//setting to circular mode
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;		//high priority
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//source size halfword=162bit
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			//destination size halfword=162bit
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	//memory destination increment enable
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//source address increment disable
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	//peripheral is source
    DMA_InitStructure.DMA_BufferSize = buffsize_adc1;		//buffer size = Nr._of_most_channels_of_any_ADC * Nr._of_ADCs
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;	//FIFO mode disabled
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; 		//doesn't matter as FIFO is disabled
    //DMA_InitStructure.DMA_FIFOThreshold = 0;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;  				//single burst only
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;			//single burst only
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADCbuf1;					//save data in ADC buffer

    DMA_Init(DMA2_Stream0, &DMA_InitStructure);			//Init DMA stream 0 for ADC


    //DMA_ITConfig(DMA2_Stream0, DMA_IT_TC | DMA_IT_HT, ENABLE); //Enable DMA Channel 0 Transfer Complete interrupt
    //DMA Interrupt
    DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);


    DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);
    //    DMA_ClearFlag(DMA2_Stream0,DMA_IT_TCIF0);DMA_FLAG_TCIFx
    DMA_ITConfig(DMA2_Stream0, DMA_IT_TC,ENABLE);
    DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);
    //    DMA_ClearFlag(DMA2_Stream0,DMA_IT_TCIF0);

    DMA_Cmd(DMA2_Stream0, ENABLE); 						//Enable the DMA Stream0

    // wait for the DMA Stream to be ready
    while(DMA_GetCmdStatus(DMA2_Stream0)==DISABLE){};
}


/*============================================================================*/
/**
 * @brief      Configuration of DMA for ADC  data transfer
 * @details    DMA for ADC on: DMA2_Stream0 and DMA for
 * 			   Size of buffsize_adc  is configured to the actual setup.
 * 			   e.g. ADC buffer size = Nr._of_most_channels_of_any_ADC * Nr._of_ADCs
 *			   e.g.: ADC1 has 3 channels : buffsize_adc is 3
 *			   readout for this case will be: ADC1_channel1, ADC1_channel2, ADC1_channel3..
 */
void DMA_Configuration_control_loop(void){
//	DMA_DeInit(DMA2_Stream0);  								//Set DMA registers to default values
	DMA_InitTypeDef  DMA_InitStructure;						//create DMA structure

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    //DMA for ADC


       DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(ADC1_BASE + 0x308);	//source DAC's Data Register + Offset
       DMA_InitStructure.DMA_Channel = DMA_Channel_7;			//set to channel 7
       DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;			//setting to circular mode
       DMA_InitStructure.DMA_Priority = DMA_Priority_High;		//high priority
       DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;	// 32-bit 16x2
       DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
       DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	//memory destination increment enable
       DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//source address increment disable
       DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;	//peripheral is source
       DMA_InitStructure.DMA_BufferSize = buffsize_adc1;		// 16-bit x 64 / 2
       DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;	//FIFO mode disabled
       DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; 		//doesn't matter as FIFO is disabled
       //DMA_InitStructure.DMA_FIFOThreshold = 0;
       DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;  				//single burst only
       DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;			//single burst only
       DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADCbuf1;					//save data in DAC buffer

       DMA_Init(DMA1_Stream5, &DMA_InitStructure);			//Init DMA stream 5 for DAC

       //DMA_ITConfig(DMA2_Stream0, DMA_IT_TC | DMA_IT_HT, ENABLE); //Enable DMA Channel 0 Transfer Complete interrupt
       //DMA Interrupt



       DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF0);
       //    DMA_ClearFlag(DMA2_Stream0,DMA_IT_TCIF0);DMA_FLAG_TCIFx
       DMA_ITConfig(DMA1_Stream5, DMA_IT_TC,ENABLE);
       DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF0);
       //    DMA_ClearFlag(DMA2_Stream0,DMA_IT_TCIF0);

       DMA_Cmd(DMA1_Stream5, ENABLE); 						//Enable the DMA Stream7

       // wait for the DMA Stream to be ready
       while(DMA_GetCmdStatus(DMA1_Stream5)==DISABLE){};
}

/**
 * @}
 */

