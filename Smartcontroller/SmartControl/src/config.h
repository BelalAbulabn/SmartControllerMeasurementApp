


/* ===========================================================================
 * Define to prevent recursive inclusion
 * ===========================================================================*/
#ifndef CONFIG_H_
#define CONFIG_H_

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
uint16_t Return_ADCbuf1(int adc_buf_position);
void usartConfig(void);
void Config_Timer4_init(void);
void Config_ADCtimer_init(void);
void Config_DACtimer_init(void);
void Config_controlLooptimer_init(void);
void DMA_Configuration(void);
void NVIC_USART_Configuration(void);
void NVIC_DMA_Configuration(void);
void NVIC_controlLoop_Configuration(void);
void DMA2_Stream0_IRQHandler(void);
void EnableTimerInterrupt(void);
void ADC_Configuration(void);
void ADC_Configuration_Averaging(void);
void ADC_Configuration_Three_Channels(void);

void DAC_Configuration(void);
void ADC_IRQHandler(void);
void SetControlLoopCallback(void (*CallbackFunction)(void));
uint8_t Return_New_ADC_conversion_available(void);
void Reset_New_ADC_conversion_available(void);
uint8_t Return_New_ADC_conversion_available_2(void);
void Reset_New_ADC_conversion_available_2(void);
uint8_t Return_Next_controlLoop(void);
void Reset_Next_controlLoop(void);
uint16_t Return_buffsize_adc (void);

#endif /* CONFIG_H_ */

/* ===========================================================================
 * Log (Place comments about changes of the file below)
 * ===========================================================================*/

/**
 * @}
 */














