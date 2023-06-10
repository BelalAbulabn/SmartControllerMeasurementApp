


 /*
 * @file    main.c
 * @authors Belal Abulabn
 * @version 1.1
 * @date    2021-10-14

 */

#include <stdio.h>


#include <string.h>
#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_dac.h"
#include "config.h"
#include "gpio.h"
#include "control.h"
#include "math.h"
#include "pwm.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stdlib.h"
#include "inttypes.h"


/*============================================================================*/
/**
 * @brief      Main function with infinite loop.
 * @return     it will never return
 * @details    The main calls an init function and the infinite mainLoop()
 * 			   which will only read new user inputs. A separate controlLoop()
 * 			   (which can be enabled by a user command handles all other functions.
 */
int main(void) {

	init_SmartControl();
	// main loop
	while (1){
		if (Return_commandReceived() != 0) usartReceived();
	}
	return 1;
}



