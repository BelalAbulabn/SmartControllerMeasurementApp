/*============================================================================*/
/**
 /*
 * @file    gbio.c
 * @authors Belal Abulabn
 * @version 1.1
 * @date    2021-10-14
 * Ein Groﬂteil der GPIO-Initialisierungen erfolgt in gpio.c
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
#include "gpio.h"
#include "math.h"
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

/* ===========================================================================
 * Local Macros
 * ===========================================================================*/

/* ===========================================================================
 * Local Type-Definitions
 * ===========================================================================*/

/* ===========================================================================
 * Local Constants
 * ===========================================================================*/

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
 * @brief      Configuration of all GPIO pins needed for SmartControl-V1.0.
 * @details    Configures LEDs, analog/ digital IN- and Outputs and BREAK-pins.
 */
void GPIO_Configuration(void) {

	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure); // Reset init structure, if not it can cause issues...

	//LED: Configure PD10 (LEDred), PD11(LEDgreen), PD12(LEDyellow) and PD13(LEDblue) in output pushpull mode
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	//ANALOG INPUTS
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3; 	// ADC: PA0, PA1, PA2, PA3 for test purposes
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;			// analog input mode for ADC
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4; 	// ADC: PC0, PC1, PC2, PC3, PC4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;     	// analog input mode for ADC
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//DIGITAL OUTPUTS
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6; 	// DO: PE1, PE2, PE3, PE4, PE5, PE6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			// digital output mode for DO
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 	// DO: PE1, PE2, PE3, PE4, PE5, PE6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			// digital output mode for DO
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//DIGITAL INPUTS
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12; 	// DI: PA8, PA11, PA12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			// digital input mode for DI
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 	// DI: PC10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			// digital input mode for DI
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//ANALOG OUTPUTS
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5; 	// DAC: PA4, PA5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;			// analog output mode for DAC
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Break_IN: Hardware Input Pins to generate Break Event -> Set_PWM_Break_Event()
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;			 	// TIM8 Break: PA6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			// digital input mode for Break_IN
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;			// Pull-Down
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;			 	// TIM1 Break: PE15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			// digital input mode for Break_IN
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;			// Pull-Down
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}


