/*
 * rcc.c
 *
 *  Created on: Mar 21, 2026
 *      Author: aravindhan-tv
 */

#include "rcc.h"

void rcc_init(void){
	/*Enable Clock for GPIOA*/
	RCC->AHB1ENR |= (1<< 0);

	//Enable clock for USART2
	RCC->APB1ENR |= (1<<17);

	//Enable clock for TIMER2 (PWN)
	RCC->APB1ENR |= (1<<0);

}
