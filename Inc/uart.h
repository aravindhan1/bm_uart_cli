/*
 * uart.h
 *
 *  Created on: Mar 21, 2026
 *      Author: aravindhan-tv
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f401xe.h"
#include "ring_buffer.h"

void uart_init(void);
void uart_send_char(char c);
void uart_send_string(const char *str);
int  uart_read_char(char *c);

#endif /* UART_H_ */
