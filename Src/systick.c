/*
 * systick.c
 *
 *  Created on: Mar 21, 2026
 *      Author: aravindhan-tv
 */
#include "systick.h"

volatile uint32_t tick =0;

void systick_init(void){
	/*
	 * Systick clock = 16Mhz (HSI default)
	 * We want 1ms tick
	 * So reload value = 16Mhz / 1000 - 1 = 15999
	 * */

	SysTick->LOAD = 15999;

	//Reset the counter to 0
	SysTick->VAL = 0;

	/*
	 * Enable Systick
	 * Bit 0 = ENABLE
	 * Bit 1 = TICKINT (enable interrupt)
	 * Bit 2 = CLKSOURCE (use processor clock)
	 * */
	SysTick->CTRL = (1<<0) | (1<<1) | (1<<2);
}

uint32_t systick_get_tick(void){
	return tick;
}

//This fucntion is called autoamtically every 1 ms
void SysTick_Handler(void){
	tick++;
}
