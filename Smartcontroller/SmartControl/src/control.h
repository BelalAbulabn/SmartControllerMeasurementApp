


/* ===========================================================================
 * Define to prevent recursive inclusion
 * ===========================================================================*/
#ifndef CONTROL_H_
#define CONTROL_H_

/* ===========================================================================
 * Includes
 * ===========================================================================*/
#include "stm32f4xx.h"

/* ===========================================================================
 * Public Defines
 * ===========================================================================*/

/* ===========================================================================
 * Public Macros
 * ===========================================================================*/

/* ===========================================================================
 * Public Type-Definitions
 * ===========================================================================*/

/* ===========================================================================
 * Public Constants
 * ===========================================================================*/

/* ===========================================================================
 * Public Variables
 * ===========================================================================*/

/* ===========================================================================
 * Declaration of Public Functions
 * ===========================================================================*/
void init_SmartControl(void);
void controlLoop(void);
void dummyLoop(void);
float ADC_single_readout_averaging_and_correction();
void DAC_Set_Output( uint16_t DAC2_Value, uint16_t DAC1_Value);
void EN_PWR(void);
void Disable_PWR(void);
void usartReceived(void);
void USART1_IRQHandler(void);
float Return_Duty_Float (void);
u8 Return_commandReceived (void);
void fputc_f(unsigned char c);
void Digital_Outputs(void);
void Digital_Inputs(void);
void Print_Status(void);
void ADC_Save_Values_In_Cache(uint32_t cachsize_s);
void ADC_Print_Values_In_Cache(uint32_t cachsize_r);
void Delay(__IO uint32_t nCount);
void togglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void interesting_led(void);
void ADC_Sampling(int Samplingfrequenz,int Numberofsamplis);
void STM_Overheat_Protection(void);
void VREFINT_measurement(void);
float Return_PWM_Mode(void);
void USART_SendFloat(float k);
void DMA_Configuration_control_loop(void);
void ADC_Configuration_Two_Channles_Coil_Current(void);
void USART_SendText(volatile char *s);
void USART_SendNumber(uint32_t x);
#endif /* CONTROL_H_ */

/* ===========================================================================
 * Log (Place comments about changes of the file below)
 * ===========================================================================*/

/**
 * @}
 */




