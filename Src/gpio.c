/*
 * gpio.c
 *
 *  Created on: Mar 21, 2026
 *      Author: aravindhan-tv
 */

#include "gpio.h"

void gpio_init(void){

	//Set PA5 as output mode
	// MODER5 = 01 -> bits 11:10 = 01
	GPIOA->MODER &= ~(3 << 10); //Clear both bits first
	GPIOA->MODER |= (1 << 10); //Set bit 10 -> Output
}

void led_on(void){
	GPIOA->ODR |= (1<<5); //Set bit 5 -> PA5 High -> Led On

}

void led_off(void){
	GPIOA->ODR &= ~(1<<5); //Clear bit 5 -> PA5 Low -> Led off

}

void led_toggle(void)
{
    GPIOA->ODR ^= (1 << 5);       /* flip bit 5 → toggles LED */
}
