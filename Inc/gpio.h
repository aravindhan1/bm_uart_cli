/*
 * gpio.h
 *
 *  Created on: Mar 21, 2026
 *      Author: aravindhan-tv
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f401xe.h"

void gpio_init(void);
void led_on(void);
void led_off(void);
void led_toggle(void);

#endif /* GPIO_H_ */
