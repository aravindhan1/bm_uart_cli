/*
 * ring_buffer.h
 *
 *  Created on: Mar 21, 2026
 *      Author: aravindhan-tv
 */

#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include <stdint.h>

#define RB_SIZE 64

typedef struct{
	char		buffer[RB_SIZE];
	uint32_t	head;
	uint32_t	tail;
}ring_buffer_t;

void rb_init(ring_buffer_t *rb);
void rb_write(ring_buffer_t *rb, char c);
int rb_read(ring_buffer_t *rb, char *c);
int rb_available(ring_buffer_t *rb);

#endif /* RING_BUFFER_H_ */
