/*============================================================================*/
/**
 *  /*
 * @file    pwm.c
 * @authors Belal Abulabn
 * @version 1.1
 * @date    2021-10-14
 * Die Steuerung der beiden Vollbrücken wird in den Funktionen der Datei pwm.c übernom-men.
 *  Die Änderung der PWM-Geschwindigkeit kann durch Anpassen des tim_period-Wertes erfolgen.

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
#include "config.h"
#include "control.h"
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

int Frequnzy=0;
int y=0;
int x=0;
int durationOgJumpFunction=0;

/* ===========================================================================
 * Local Macros
 * ===========================================================================*/

/* ===========================================================================
 * Local Type-Definitions
 * ===========================================================================*/

/* ===========================================================================
 * Local Constants
 * ===========================================================================*/
static int tim_period = 8400;	// change into an even number in order to get different PWM frequencies; 8400=20kHz; 4200=40kHz; 3360=50kHz
static int dead_time = 0x22;	// Dead Time in seconds = value/168MHz; e.g.: dead_time = 70 is 416.7 ns, 0x11 = 101 ns; real value may be slightly higher!

int pwm_mode_coil2 = 0;



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
 * @brief      Timer 4 initialization with interrupt
 * @details    is used for (Sprungsiganle)
 */
void Config_Timer4_init(void)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

	    // Set module-variables
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		RCC_ClocksTypeDef   RCC_ClocksStatus;
		RCC_GetClocksFreq(&RCC_ClocksStatus);
		/* Enable the TIM4 global Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn ;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 10;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		/* Enable the TIM4 */
		TIM_TimeBaseStructure.TIM_Period = (1000-1);
		TIM_TimeBaseStructure.TIM_Prescaler = ((2*RCC_ClocksStatus.PCLK1_Frequency)/10000-1);
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

		/* TIM Interrupts enable */
	    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

	    TIM_Cmd(TIM4, ENABLE);
}

/*============================================================================*/
/**
 * @brief      Interrupt Handler for TIM4 (controlLoop)
 * @details    Waits for TIM4 period to deactivate the Coil_1 und Coil_2
 */

void TIM4_IRQHandler(void) {
FlagStatus temp;
	temp = TIM_GetITStatus(TIM4, TIM_IT_Update);
	//temp = DMA_GetFlagStatus(DMA2_Stream0, DMA_FLAG_TCIF0);
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update); // Clears the DMA's transfer complete interrupt
	//DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0); // Clears the DMA's transfer complete interrupt

		if (temp!=0) {
			//TIM8->CCR1 = (uint32_t)55437;

			x++;
			if(x<durationOgJumpFunction)
					{
				//	GPIO_ToggleBits(GPIOC, GPIO_Pin_11);
					//TIM1->CCR1 = (uint32_t)	55437;


					}
				if(x==durationOgJumpFunction) {
					TIM_CtrlPWMOutputs(TIM1, DISABLE);
					TIM_CtrlPWMOutputs(TIM8, DISABLE);
					y++;
					x=0;

						}
					}





}



/*============================================================================*/
/**
 * @brief      Sets (and resets) the pwm_operation_mode value after initializing TIM1 and/or TIM8
 * @param 	   i_id is comparable to pwm_operation_mode
 * @details    Sets the duty cycle for Coil_1 and Coil_2 in fast decay mode (pwm_operation_mode must be 1,2 or 3).
 * 			   pwm_operation_mode = 0 disables the PWM output
 * 			   Init_PWM_Operation_Mode() inits TIM1 and/ or TIM8. Once that is done, Set_Duty_Cycle() can change duty cycles.
 */
void Init_PWM_Operation_Mode(int i_id){

	TIM_OCInitTypeDef 		TIM_OCInitStructure;
	TIM_BDTRInitTypeDef     TIM_BDTRInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;


	TIM_DeInit (TIM1);
	TIM_DeInit (TIM8);

	TIM_TimeBaseStructure.TIM_Period = tim_period; // 0xFFF=390µs/2,56kHz; 8400/0x20D0=20kHz  Check TIM_Pulse as well!!
	TIM_TimeBaseStructure.TIM_Prescaler = 0; // 0xFFFF ~1.28kHz 0x07FF ~41kHz 0x1FFF ~ 20 kHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);		//TIM8 basic config: same as TIM1.

	//Automatic Output enable, Break, dead time and lock configuration
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;   //TIM_LOCKLevel_OFF
	TIM_BDTRInitStructure.TIM_DeadTime = dead_time;            //0x0f -> deadtime = value/168MHz
	TIM_BDTRInitStructure.TIM_Break =  TIM_Break_Enable;       //TIM1_Break_Enable // break input off?
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable; //TIM_AutomaticOutput_Disable; //AOE off?
	TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
	TIM_BDTRConfig(TIM8, &TIM_BDTRInitStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_Pulse = 0;              			//Duty-Cycle 0 is okay when all are in TIM_OCMode_PWM1

	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);


	if (i_id == 0){
		//disable all PWM channels:
		TIM_CtrlPWMOutputs(TIM1, DISABLE);
		TIM_CtrlPWMOutputs(TIM8, DISABLE);
		printf("\r PWM successfully disabled!");
		printf("\r Please Init again and set a new duty cycle to start PWM.");
	}
	else if ((i_id > 0) && (i_id<=3)){
		if (i_id == 1){		//TIM1Ch1 and TIM1Ch1N + TIM1Ch2 and TIM1Ch2N


			TIM_OCInitStructure.TIM_Pulse = (int)(tim_period/2); 	//Duty-Cycle fast decay mode, complementary signal, no current
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
			TIM_OC2Init(TIM1, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
			//set normal channel second
			TIM_OCInitStructure.TIM_Pulse = (int)(tim_period/2);   	//Duty-Cycle fast decay mode, non-inverted signal, no current
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
			TIM_OC1Init(TIM1, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
		    /* Set the ARR Preload Bit */
			TIM1->CR1 |= TIM_CR1_ARPE;
		    /* Enable the TIM Counter */
		    TIM1->CR1 |= TIM_CR1_CEN;
			/* Enable the TIM Main Output */
		    TIM1->BDTR |= TIM_BDTR_MOE;
			printf("New configuration loaded. Mode %i", i_id);
		}
		else if (i_id == 2){		//TIM1Ch3 and TIM1Ch3N + TIM8Ch1 and TIM8Ch1N


			// set complementary channel first, than desired normal channel
			// duty cycle (pulse) configuration
			TIM_OCInitStructure.TIM_Pulse = (int)(tim_period/2); 	//Duty-Cycle fast decay mode, complementary signal, no current
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
			TIM_OC1Init(TIM8, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
			// set normal channel second
			TIM_OCInitStructure.TIM_Pulse = (int)(tim_period/2);   	//Duty-Cycle fast decay mode, non-inverted signal, no current
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
			TIM_OC3Init(TIM1, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
			/* Set the ARR Preload Bit */
			TIM8->CR1 |= TIM_CR1_ARPE;
			TIM1->CR1 |= TIM_CR1_ARPE;
		    /* Enable the TIM Counter */
		    TIM8->CR1 |= TIM_CR1_CEN;
		    TIM1->CR1 |= TIM_CR1_CEN;
			/* Enable the TIM Main Output */
		    TIM8->BDTR |= TIM_BDTR_MOE;
			TIM1->BDTR |= TIM_BDTR_MOE;
			printf("New configuration loaded. Mode %i", i_id);
		}
		else if (i_id == 3){		//TIM1Ch1 and TIM1Ch1N + TIM1Ch2 and TIM1Ch2N & TIM1Ch3 and TIM1Ch3N + TIM8Ch1 and TIM8Ch1N


			TIM_OCInitStructure.TIM_Pulse = (int)(tim_period/2); 	//Duty-Cycle fast decay mode, complementary signal, no current
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
			TIM_OC2Init(TIM1, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
			TIM_OC1Init(TIM8, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
			// set normal channels second
			TIM_OCInitStructure.TIM_Pulse = (int)(tim_period/2);   	//Duty-Cycle fast decay mode, non-inverted signal, no current
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
			TIM_OC1Init(TIM1, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
			TIM_OC3Init(TIM1, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
		    /* Set the ARR Preload Bit */
			TIM8->CR1 |= TIM_CR1_ARPE;
			TIM1->CR1 |= TIM_CR1_ARPE;
		    /* Enable the TIM Counter */
		    TIM8->CR1 |= TIM_CR1_CEN;
		    TIM1->CR1 |= TIM_CR1_CEN;
			/* Enable the TIM Main Output */
		    TIM8->BDTR |= TIM_BDTR_MOE;
			TIM1->BDTR |= TIM_BDTR_MOE;
		}
		else{
		}
	}
	else {
	}
}




void Init_PWM_Operation_Full_Bridig(int i_id,int Frequnzy,int DutyCycle){
	 PWM_init();

	TIM_OCInitTypeDef 		TIM_OCInitStructure;
	TIM_BDTRInitTypeDef     TIM_BDTRInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_CtrlPWMOutputs(TIM4, DISABLE);

	TIM_DeInit (TIM1);
	TIM_DeInit (TIM8);

	TIM_TimeBaseStructure.TIM_Period = Frequnzy; // 0xFFF=390µs/2,56kHz; 8400/0x20D0=20kHz  Check TIM_Pulse as well!!
	TIM_TimeBaseStructure.TIM_Prescaler = 0; // 0xFFFF ~1.28kHz 0x07FF ~41kHz 0x1FFF ~ 20 kHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	//TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);		//TIM8 basic config: same as TIM1.

	//Automatic Output enable, Break, dead time and lock configuration
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;   //TIM_LOCKLevel_OFF
	TIM_BDTRInitStructure.TIM_DeadTime = dead_time;            //0x0f -> deadtime = value/168MHz
	TIM_BDTRInitStructure.TIM_Break =  TIM_Break_Enable;       //TIM1_Break_Enable // break input off?
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable; //TIM_AutomaticOutput_Disable; //AOE off?
	TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
	//TIM_BDTRConfig(TIM8, &TIM_BDTRInitStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_Pulse = 0;              			//Duty-Cycle 0 is okay when all are in TIM_OCMode_PWM1

	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	//TIM_OC1Init(TIM8, &TIM_OCInitStructure);


	if (i_id == 0){
		//disable all PWM channels:
		TIM_CtrlPWMOutputs(TIM1, DISABLE);
		TIM_CtrlPWMOutputs(TIM8, DISABLE);
		printf("\r PWM successfully disabled!");
		printf("\r Please Init again and set a new duty cycle to start PWM.");
	}
	else if ((i_id > 0) && (i_id<=3)){
		if (i_id == 1){		//TIM1Ch1 and TIM1Ch1N + TIM1Ch2 and TIM1Ch2N


			TIM_OCInitStructure.TIM_Pulse = (int)DutyCycle; 	//Duty-Cycle fast decay mode, complementary signal, no current
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
			TIM_OC2Init(TIM1, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
			//set normal channel second
			TIM_OCInitStructure.TIM_Pulse = (int)DutyCycle;   	//Duty-Cycle fast decay mode, non-inverted signal, no current
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
			TIM_OC1Init(TIM1, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
		    /* Set the ARR Preload Bit */
			TIM1->CR1 |= TIM_CR1_ARPE;
		    /* Enable the TIM Counter */
		    TIM1->CR1 |= TIM_CR1_CEN;
			/* Enable the TIM Main Output */
		    TIM1->BDTR |= TIM_BDTR_MOE;
			// set PWM duty cycle TIM1 CC1 and CC2
				TIM1->CCR1 = (uint32_t)DutyCycle;
				TIM1->CCR2 = (uint32_t)DutyCycle;


		}
		else if (i_id == 2){		//TIM1Ch3 and TIM1Ch3N + TIM8Ch1 and TIM8Ch1N




			TIM_OCInitStructure.TIM_Pulse = (int)(DutyCycle); 	//Duty-Cycle fast decay mode, complementary signal, no current
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
			TIM_OC1Init(TIM8, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
			// set normal channel second
			TIM_OCInitStructure.TIM_Pulse = (int)(DutyCycle);   	//Duty-Cycle fast decay mode, non-inverted signal, no current
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
			TIM_OC3Init(TIM1, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
			/* Set the ARR Preload Bit */
			TIM8->CR1 |= TIM_CR1_ARPE;
			TIM1->CR1 |= TIM_CR1_ARPE;
		    /* Enable the TIM Counter */
		    TIM8->CR1 |= TIM_CR1_CEN;
		    TIM1->CR1 |= TIM_CR1_CEN;
			/* Enable the TIM Main Output */
		    TIM8->BDTR |= TIM_BDTR_MOE;
			TIM1->BDTR |= TIM_BDTR_MOE;
			printf("New configuration loaded. Mode %i", i_id);
		}
		else if (i_id == 3){		//TIM1Ch1 and TIM1Ch1N + TIM1Ch2 and TIM1Ch2N & TIM1Ch3 and TIM1Ch3N + TIM8Ch1 and TIM8Ch1N


			TIM_OCInitStructure.TIM_Pulse = (int)(DutyCycle); 	//Duty-Cycle fast decay mode, complementary signal, no current
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
			TIM_OC2Init(TIM1, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
			TIM_OC1Init(TIM8, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
			// set normal channels second
			TIM_OCInitStructure.TIM_Pulse = (int)(DutyCycle);   	//Duty-Cycle fast decay mode, non-inverted signal, no current
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
			TIM_OC1Init(TIM1, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
			TIM_OC3Init(TIM1, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
		    /* Set the ARR Preload Bit */
			TIM8->CR1 |= TIM_CR1_ARPE;
			TIM1->CR1 |= TIM_CR1_ARPE;
		    /* Enable the TIM Counter */
		    TIM8->CR1 |= TIM_CR1_CEN;
		    TIM1->CR1 |= TIM_CR1_CEN;
			/* Enable the TIM Main Output */
		    TIM8->BDTR |= TIM_BDTR_MOE;
			TIM1->BDTR |= TIM_BDTR_MOE;
		}
		else{
		}
	}
	else {
	}
		printf("\rPWM Output Enabled.");
}
//it does not work
void Init_Trapez(int i_id,int x){
//almost the slow signals that are implemented with prescaler 0.
	TIM_OCInitTypeDef 		TIM_OCInitStructure;
	TIM_BDTRInitTypeDef     TIM_BDTRInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;


	TIM_DeInit (TIM1);
	TIM_DeInit (TIM8);

	TIM_TimeBaseStructure.TIM_Period = x; // 0xFFF=390µs/2,56kHz; 8400/0x20D0=20kHz  Check TIM_Pulse as well!!
	TIM_TimeBaseStructure.TIM_Prescaler = 0; // 0xFFFF ~1.28kHz 0x07FF ~41kHz 0x1FFF ~ 20 kHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);		//TIM8 basic config: same as TIM1.

	//Automatic Output enable, Break, dead time and lock configuration
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;   //TIM_LOCKLevel_OFF
	TIM_BDTRInitStructure.TIM_DeadTime = dead_time;            //0x0f -> deadtime = value/168MHz
	TIM_BDTRInitStructure.TIM_Break =  TIM_Break_Enable;       //TIM1_Break_Enable // break input off?
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable; //TIM_AutomaticOutput_Disable; //AOE off?
	TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
	TIM_BDTRConfig(TIM8, &TIM_BDTRInitStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_Pulse = 0;              			//Duty-Cycle 0 is okay when all are in TIM_OCMode_PWM1

	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);


	if (i_id == 0){
		//disable all PWM channels:
		TIM_CtrlPWMOutputs(TIM1, DISABLE);
		TIM_CtrlPWMOutputs(TIM8, DISABLE);

	}
	else if ((i_id > 0) && (i_id<=3)){
		if (i_id == 1){		//TIM1Ch1 and TIM1Ch1N + TIM1Ch2 and TIM1Ch2N


			for(double v=1;v<=90 ;v++){
				double y=x*(v/100);
				Delay(1680000*3);
			TIM_OCInitStructure.TIM_Pulse = (double)y; 	//Duty-Cycle fast decay mode, complementary signal, no current
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
			TIM_OC2Init(TIM1, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
			//set normal channel second
			TIM_OCInitStructure.TIM_Pulse = (double)y;   	//Duty-Cycle fast decay mode, non-inverted signal, no current
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
			TIM_OC1Init(TIM1, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
		    /* Set the ARR Preload Bit */
			TIM1->CR1 |= TIM_CR1_ARPE;
		    /* Enable the TIM Counter */
		    TIM1->CR1 |= TIM_CR1_CEN;
			/* Enable the TIM Main Output */
		    TIM1->BDTR |= TIM_BDTR_MOE;
			printf("New configuration loaded. Mode %i", i_id);

		}
		}
		else if (i_id == 2){		//TIM1Ch3 and TIM1Ch3N + TIM8Ch1 and TIM8Ch1N


			TIM_OCInitStructure.TIM_Pulse = (int)(tim_period/2); 	//Duty-Cycle fast decay mode, complementary signal, no current
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
			TIM_OC1Init(TIM8, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
			// set normal channel second
			TIM_OCInitStructure.TIM_Pulse = (int)(tim_period/2);   	//Duty-Cycle fast decay mode, non-inverted signal, no current
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
			TIM_OC3Init(TIM1, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
			/* Set the ARR Preload Bit */
			TIM8->CR1 |= TIM_CR1_ARPE;
			TIM1->CR1 |= TIM_CR1_ARPE;
		    /* Enable the TIM Counter */
		    TIM8->CR1 |= TIM_CR1_CEN;
		    TIM1->CR1 |= TIM_CR1_CEN;
			/* Enable the TIM Main Output */
		    TIM8->BDTR |= TIM_BDTR_MOE;
			TIM1->BDTR |= TIM_BDTR_MOE;
			printf("New configuration loaded. Mode %i", i_id);
		}
		else if (i_id == 3){		//TIM1Ch1 and TIM1Ch1N + TIM1Ch2 and TIM1Ch2N & TIM1Ch3 and TIM1Ch3N + TIM8Ch1 and TIM8Ch1N


			// duty cycle (pulse) configuration
			TIM_OCInitStructure.TIM_Pulse = (int)(tim_period/2); 	//Duty-Cycle fast decay mode, complementary signal, no current
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
			TIM_OC2Init(TIM1, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
			TIM_OC1Init(TIM8, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
			// set normal channels second
			TIM_OCInitStructure.TIM_Pulse = (int)(tim_period/2);   	//Duty-Cycle fast decay mode, non-inverted signal, no current
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
			TIM_OC1Init(TIM1, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
			TIM_OC3Init(TIM1, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
		    /* Set the ARR Preload Bit */
			TIM8->CR1 |= TIM_CR1_ARPE;
			TIM1->CR1 |= TIM_CR1_ARPE;
		    /* Enable the TIM Counter */
		    TIM8->CR1 |= TIM_CR1_CEN;
		    TIM1->CR1 |= TIM_CR1_CEN;
			/* Enable the TIM Main Output */
		    TIM8->BDTR |= TIM_BDTR_MOE;
			TIM1->BDTR |= TIM_BDTR_MOE;
		}
		else{
		}
	}
	else {
	}
		printf("\rPWM Output Enabled.");
}
uint32_t TIM_GetT_Period(TIM_TypeDef* TIMx)
{
  /* Check the parameters */
  assert_param(IS_TIM_LIST1_PERIPH(TIMx));

  /* Get the Capture 1 Register value */
  return TIMx->ARR;
}
void Init_PWM_Operation_Halb_Bridig(int i_id,int Frequnzy,int DutyCycle){

	TIM_OCInitTypeDef 		TIM_OCInitStructure;

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;


	TIM_CtrlPWMOutputs(TIM4, DISABLE);
    TIM_Cmd(TIM4, DISABLE);

	//reset all necessary settings in order to keep the code working
	TIM_DeInit (TIM1);
    TIM_DeInit (TIM8);

    PWM_init_halb_Bridig();
	TIM_TimeBaseStructure.TIM_Period = Frequnzy;
	TIM_TimeBaseStructure.TIM_Prescaler = 0; // 0xFFFF ~1.28kHz 0x07FF ~41kHz 0x1FFF ~ 20 kHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);		//TIM8 basic config: same as TIM1.
	TIM_GetCapture1(TIM1);
	TIM_GetCounter(TIM1);
	TIM_GetT_Period(TIM1);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_Pulse = 0;              			//Duty-Cycle 0 is okay when all are in TIM_OCMode_PWM1

	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState =  TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity =  TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState =  TIM_OCNIdleState_Set;



	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);



	if (i_id == 0){
		//disable all PWM channels:
		TIM_CtrlPWMOutputs(TIM1, DISABLE);
		TIM_CtrlPWMOutputs(TIM8, DISABLE);
		printf("\r PWM successfully disabled!");
		printf("\r Please Init again and set a new duty cycle to start PWM.");
	}
	else if ((i_id > 0) && (i_id<=3)){
		if (i_id == 1){		//TIM1Ch1 and TIM1Ch1N + TIM1Ch2 and TIM1Ch2N




			TIM1->CR1 |= TIM_CR1_ARPE;
		    /* Enable the TIM Counter */
		    TIM1->CR1 |= TIM_CR1_CEN;
			/* Enable the TIM Main Output */
		    TIM1->BDTR |= TIM_BDTR_MOE;
			// set PWM duty cycle TIM1 CC1 and CC2

			TIM1->CCR1 = (uint32_t)	DutyCycle;




			printf("New configuration loaded. Mode %i", i_id);

		}
		else if (i_id == 2){		//TIM1Ch3 and TIM1Ch3N + TIM8Ch1 and TIM8Ch1N




			// set complementary channel first, than desired normal channel
			// duty cycle (pulse) configuration

			/* Set the ARR Preload Bit */
			TIM8->CR1 |= TIM_CR1_ARPE;
			TIM1->CR1 |= TIM_CR1_ARPE;
		    /* Enable the TIM Counter */
		    TIM8->CR1 |= TIM_CR1_CEN;
		    TIM1->CR1 |= TIM_CR1_CEN;
			/* Enable the TIM Main Output */
		    TIM8->BDTR |= TIM_BDTR_MOE;
			TIM1->BDTR |= TIM_BDTR_MOE;
			TIM8->CCR1 = (uint32_t)DutyCycle;

			printf("New configuration loaded. Mode %i", i_id);
		}
		else if (i_id == 3){		//TIM1Ch1 and TIM1Ch1N + TIM1Ch2 and TIM1Ch2N & TIM1Ch3 and TIM1Ch3N + TIM8Ch1 and TIM8Ch1N


		    /* Set the ARR Preload Bit */
			TIM8->CR1 |= TIM_CR1_ARPE;
			TIM1->CR1 |= TIM_CR1_ARPE;
		    /* Enable the TIM Counter */
		    TIM8->CR1 |= TIM_CR1_CEN;
		    TIM1->CR1 |= TIM_CR1_CEN;
			/* Enable the TIM Main Output */
		    TIM8->BDTR |= TIM_BDTR_MOE;
			TIM1->BDTR |= TIM_BDTR_MOE;
			TIM1->CCR1 = (uint32_t)DutyCycle;
			TIM8->CCR1 = (uint32_t)DutyCycle;

		}
		else{
		}
	}
	else {
	}
}



void Init_PWM_Operation(int i_id,int Frequnzy,int DutyCycle){

	TIM_OCInitTypeDef 		TIM_OCInitStructure;

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;





	//reset all necessary settings in order to keep the code working
	TIM_DeInit (TIM1);
    TIM_DeInit (TIM8);
    RCC_ClocksTypeDef   RCC_ClocksStatus;
    		RCC_GetClocksFreq(&RCC_ClocksStatus);
    PWM_init_halb_Bridig();
	TIM_TimeBaseStructure.TIM_Period = 277; // 0xFFF=390µs/2,56kHz; 8400/0x20D0=20kHz  Check TIM_Pulse as well!!
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);		//TIM8 basic config: same as TIM1.



	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_Pulse = 0;              			//Duty-Cycle 0 is okay when all are in TIM_OCMode_PWM1

	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState =  TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity =  TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState =  TIM_OCNIdleState_Set;



	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);



	if (i_id == 0){
		//disable all PWM channels:
		TIM_CtrlPWMOutputs(TIM1, DISABLE);
		TIM_CtrlPWMOutputs(TIM8, DISABLE);
		printf("\r PWM successfully disabled!");
		printf("\r Please Init again and set a new duty cycle to start PWM.");
	}
	else if ((i_id > 0) && (i_id<=3)){
		if (i_id == 1){		//TIM1Ch1 and TIM1Ch1N + TIM1Ch2 and TIM1Ch2N



			TIM1->CR1 |= TIM_CR1_ARPE;
		    /* Enable the TIM Counter */
		    TIM1->CR1 |= TIM_CR1_CEN;
			/* Enable the TIM Main Output */
		    TIM1->BDTR |= TIM_BDTR_MOE;
			// set PWM duty cycle TIM1 CC1 and CC2

			TIM1->CCR1 = (uint32_t)831;






		}
		else if (i_id == 2){		//TIM1Ch3 and TIM1Ch3N + TIM8Ch1 and TIM8Ch1N




			// set complementary channel first, than desired normal channel
			// duty cycle (pulse) configuration

			/* Set the ARR Preload Bit */
			TIM8->CR1 |= TIM_CR1_ARPE;
			TIM1->CR1 |= TIM_CR1_ARPE;
		    /* Enable the TIM Counter */
		    TIM8->CR1 |= TIM_CR1_CEN;
		    TIM1->CR1 |= TIM_CR1_CEN;
			/* Enable the TIM Main Output */
		    TIM8->BDTR |= TIM_BDTR_MOE;
			TIM1->BDTR |= TIM_BDTR_MOE;
			TIM8->CCR1 = (uint32_t)DutyCycle;
            //set pwm_operation_mode to coil_2
		}
		else if (i_id == 3){		//TIM1Ch1 and TIM1Ch1N + TIM1Ch2 and TIM1Ch2N & TIM1Ch3 and TIM1Ch3N + TIM8Ch1 and TIM8Ch1N


		    /* Set the ARR Preload Bit */
			TIM8->CR1 |= TIM_CR1_ARPE;
			TIM1->CR1 |= TIM_CR1_ARPE;
		    /* Enable the TIM Counter */
		    TIM8->CR1 |= TIM_CR1_CEN;
		    TIM1->CR1 |= TIM_CR1_CEN;
			/* Enable the TIM Main Output */
		    TIM8->BDTR |= TIM_BDTR_MOE;
			TIM1->BDTR |= TIM_BDTR_MOE;
			TIM1->CCR1 = (uint32_t)DutyCycle;
			TIM8->CCR1 = (uint32_t)DutyCycle;

		}
		else{
		}
	}
	else {
	}
		printf("\rPWM Output Enabled.");
}

void Sprungsignal(int i_id,int DutyCycle,int NumberaofPulses){

	    TIM_OCInitTypeDef 		TIM_OCInitStructure;

		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;


		durationOgJumpFunction=DutyCycle;
		//reset all necessary settings in order to keep the code working
		TIM_DeInit (TIM1);
	    TIM_DeInit (TIM8);
		TIM_DeInit (TIM4);

	    PWM_init_halb_Bridig();
		TIM_TimeBaseStructure.TIM_Period = 55440; // 0xFFF=390µs/2,56kHz; 8400/0x20D0=20kHz  Check TIM_Pulse as well!!
		TIM_TimeBaseStructure.TIM_Prescaler = 0; // 0xFFFF ~1.28kHz 0x07FF ~41kHz 0x1FFF ~ 20 kHz
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
		TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);		//TIM8 basic config: same as TIM1.
		TIM_GetCapture1(TIM1);
		TIM_GetCounter(TIM1);
		TIM_GetT_Period(TIM1);

		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_Pulse = 0;              			//Duty-Cycle 0 is okay when all are in TIM_OCMode_PWM1

		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_OutputNState =  TIM_OutputNState_Enable;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OCNPolarity =  TIM_OCNPolarity_High;
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCNIdleState =  TIM_OCNIdleState_Set;



		TIM_OC1Init(TIM1, &TIM_OCInitStructure);
		TIM_OC2Init(TIM1, &TIM_OCInitStructure);
		TIM_OC3Init(TIM1, &TIM_OCInitStructure);
		TIM_OC1Init(TIM8, &TIM_OCInitStructure);
		TIM_OC2Init(TIM8, &TIM_OCInitStructure);

		   Config_Timer4_init();


		if (i_id == 0){
			//disable all PWM channels:
			TIM_CtrlPWMOutputs(TIM1, DISABLE);
			TIM_CtrlPWMOutputs(TIM8, DISABLE);
			printf("\r PWM successfully disabled!");
			printf("\r Please Init again and set a new duty cycle to start PWM.");
		}
		else if ((i_id > 0) && (i_id<=3)){
			if (i_id == 1){		//TIM1Ch1 and TIM1Ch1N + TIM1Ch2 and TIM1Ch2N




				TIM1->CR1 |= TIM_CR1_ARPE;
			    /* Enable the TIM Counter */
			    TIM1->CR1 |= TIM_CR1_CEN;
				/* Enable the TIM Main Output */
			    TIM1->BDTR |= TIM_BDTR_MOE;
				// set PWM duty cycle TIM1 CC1 and CC2 bei 99.99%

				TIM1->CCR1 = (uint32_t)	55437;





				printf("New configuration loaded. Mode %i", i_id);

			}
			else if (i_id == 2){		//TIM1Ch3 and TIM1Ch3N + TIM8Ch1 and TIM8Ch1N




				// set complementary channel first, than desired normal channel
				// duty cycle (pulse) configuration

				/* Set the ARR Preload Bit */
				TIM8->CR1 |= TIM_CR1_ARPE;
				TIM1->CR1 |= TIM_CR1_ARPE;
			    /* Enable the TIM Counter */
			    TIM8->CR1 |= TIM_CR1_CEN;
			    TIM1->CR1 |= TIM_CR1_CEN;
				/* Enable the TIM Main Output */
			    TIM8->BDTR |= TIM_BDTR_MOE;
				TIM1->BDTR |= TIM_BDTR_MOE;
				TIM8->CCR1 = (uint32_t)55437;
				printf("New configuration loaded. Mode %i", i_id);
			}
			else if (i_id == 3){		//TIM1Ch1 and TIM1Ch1N + TIM1Ch2 and TIM1Ch2N & TIM1Ch3 and TIM1Ch3N + TIM8Ch1 and TIM8Ch1N


			    /* Set the ARR Preload Bit */
				TIM8->CR1 |= TIM_CR1_ARPE;
				TIM1->CR1 |= TIM_CR1_ARPE;
			    /* Enable the TIM Counter */
			    TIM8->CR1 |= TIM_CR1_CEN;
			    TIM1->CR1 |= TIM_CR1_CEN;
				/* Enable the TIM Main Output */
			    TIM8->BDTR |= TIM_BDTR_MOE;
				TIM1->BDTR |= TIM_BDTR_MOE;
				TIM1->CCR1 = (uint32_t)55437;
				TIM8->CCR1 = (uint32_t)55437;

			}
			else{
				printf("\rThere is no #X connected to %i", i_id);
			}
		}
		else {
			printf("\r Wrong #X");
		}
			printf("\rPWM Output Enabled.");


}
void PWM_init_halb_Bridig(void){

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_TIM1);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_TIM1);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

    GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

    GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_TIM1);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM8);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    //TIM1 and TIM8 PWM
    	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    	TIM_DeInit (TIM1);
    	TIM_DeInit (TIM8);


    	TIM_TimeBaseStructure.TIM_Period = tim_period; // 0xFFF=390µs/2,56kHz; 8400/0x20D0=20kHz; 4200=40kHz; 3360=50kHz; Check TIM_Pulse as well!
    	TIM_TimeBaseStructure.TIM_Prescaler = 0; // 0xFFFF ~1.28kHz 0x07FF ~41kHz 0x1FFF ~ 20 kHz
    	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

    	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);		//TIM8 basic config: same as TIM1. for fast decay see Set_Duty_Cycle()


    	TIM_OCInitTypeDef TIM_OCInitStructure;
    	/* PWM1 Mode configuration */
    	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;

    	TIM_OCInitStructure.TIM_Pulse = 0;              //Duty-Cycle, TIM1Ch1
    	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
    	TIM_OC1Init(TIM1, &TIM_OCInitStructure);

    	TIM_OCInitStructure.TIM_Pulse = 0;              //Duty-Cycle, TIM1Ch2
    	TIM_OC2Init(TIM1, &TIM_OCInitStructure);

    	TIM_OCInitStructure.TIM_Pulse = 0;              //Duty-Cycle, TIM1Ch3
    	TIM_OC3Init(TIM1, &TIM_OCInitStructure);

    	TIM_OCInitStructure.TIM_Pulse = 0;              //Duty-Cycle, TIM8Ch1
    	TIM_OC1Init(TIM8, &TIM_OCInitStructure);
    	TIM_OC2Init(TIM8, &TIM_OCInitStructure);        //Duty-Cycle, TIM8Ch2




        /* Set the ARR Preload Bit */
    	TIM8->CR1 |= TIM_CR1_ARPE;
    	TIM1->CR1 |= TIM_CR1_ARPE;
        /* Enable the TIM Counter */
        TIM8->CR1 |= TIM_CR1_CEN;
        TIM1->CR1 |= TIM_CR1_CEN;
    	/* Disable the TIM Main Output for now */
        TIM8->BDTR &= (uint16_t)~TIM_BDTR_MOE;
        TIM1->BDTR &= (uint16_t)~TIM_BDTR_MOE;
}
/*============================================================================*/
/**
 * @brief      Initialization of all PWM ports, timers and channels
 * @details    RCC_... activates the specific clocks (168 MHz for the Timers, up to 100 MHz for GPIOs),
 * 			   GPIO_... activates all PWM specific ports and
 * 			   TIM_... inits TIM1Ch1/N,TIM1Ch2/N,TIM1Ch3/N andTIM8Ch1/N as well as break, dead-time, duty cycle,...
 * 			   Only call this function on the microcontroller startup
 */
void PWM_init(void) {
//Enable PE and TIM1
//Enable PA and PC and TIM8

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

//__________TIM1_________Channel 1______________
	//PE 8,9 Output
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);


//__________TIM1_________Channel 2______________
	//PE 10,11 Output

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);


//__________TIM1_________Channel 3______________
	//PE 12,13 Output

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);


//__________TIM8_________Channel 1______________
	//PC 6, PA 7 Output

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM8);




	//TIM1 and TIM8 PWM
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_DeInit (TIM1);
	TIM_DeInit (TIM8);



	TIM_TimeBaseStructure.TIM_Period = tim_period;
	TIM_TimeBaseStructure.TIM_Prescaler = 0; // 0xFFFF ~1.28kHz 0x07FF ~41kHz 0x1FFF ~ 20 kHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);		//TIM8 basic config: same as TIM1. for fast decay see Set_Duty_Cycle()


	TIM_OCInitTypeDef TIM_OCInitStructure;
	/* PWM1 Mode configuration */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;

	TIM_OCInitStructure.TIM_Pulse = 0;              //Duty-Cycle, TIM1Ch1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_Pulse = 0;              //Duty-Cycle, TIM1Ch2
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_Pulse = 0;              //Duty-Cycle, TIM1Ch3
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_Pulse = 0;              //Duty-Cycle, TIM8Ch1
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);

	TIM_BDTRInitTypeDef     TIM_BDTRInitStructure;
	//Automatic Output enable, Break, dead time and lock configuration
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;     //TIM_LOCKLevel_OFF,TIM_LOCKLevel_1
	TIM_BDTRInitStructure.TIM_DeadTime = dead_time;                    //deadtime=0x0f or higher
	TIM_BDTRInitStructure.TIM_Break =  TIM_Break_Enable;         //TIM1_Break_Enable // break input off?
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable; //TIM_AutomaticOutput_Disable; //AOE off?
	TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
	TIM_BDTRConfig(TIM8, &TIM_BDTRInitStructure);

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);

    /* Set the ARR Preload Bit */
	TIM8->CR1 |= TIM_CR1_ARPE;
	TIM1->CR1 |= TIM_CR1_ARPE;
    /* Enable the TIM Counter */
    TIM8->CR1 |= TIM_CR1_CEN;
    TIM1->CR1 |= TIM_CR1_CEN;
	/* Disable the TIM Main Output for now */
    TIM8->BDTR &= (uint16_t)~TIM_BDTR_MOE;
    TIM1->BDTR &= (uint16_t)~TIM_BDTR_MOE;
}




