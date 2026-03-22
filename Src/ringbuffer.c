/*
 * ringbuffer.c
 *
 *  Created on: Mar 21, 2026
 *      Author: aravindhan-tv
 */

#include "ring_buffer.h"

void rb_init(ring_buffer_t *rb){
	rb->head = 0;
	rb->tail = 0;
}

//called form ISR - Writes one character
void rb_write(ring_buffer_t *rb, char c){
	uint32_t next_head = (rb->head + 1) % RB_SIZE;

	if (next_head != rb->tail){
		rb->buffer[rb->head] = c;
		rb->head = next_head;
	}
	//if full - Drpp the character
}

/* Called from CLI task — reads one character */
int rb_read(ring_buffer_t *rb, char *c)
{
    if (rb->tail == rb->head)    /* buffer empty */
        return 0;

    *c = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % RB_SIZE;
    return 1;                    /* success */
}

/* How many characters are waiting */
int rb_available(ring_buffer_t *rb)
{
    return (rb->head - rb->tail + RB_SIZE) % RB_SIZE;
}
