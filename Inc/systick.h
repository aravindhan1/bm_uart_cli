/*
 * systick.h
 *
 *  Created on: Mar 21, 2026
 *      Author: aravindhan-tv
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "stm32f401xe.h"
#include <stdint.h>


void systick_init(void);
uint32_t systick_get_tick(void);
void SysTick_Handler(void);


#endif /* SYSTICK_H_ */
