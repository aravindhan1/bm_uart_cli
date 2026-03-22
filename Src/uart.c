#include "uart.h"

/* Ring buffer for received characters */
static ring_buffer_t rx_buffer;

void uart_init(void)
{
    /* Step 1: Configure PA2 (TX) and PA3 (RX) as Alternate Function */
    /* Set MODER to Alternate Function (10) for PA2 and PA3 */
    GPIOA->MODER &= ~(0b11 << 4);   /* clear PA2 mode bits */
    GPIOA->MODER |=  (0b10 << 4);   /* PA2 = alternate function */

    GPIOA->MODER &= ~(0b11 << 6);   /* clear PA3 mode bits */
    GPIOA->MODER |=  (0b10 << 6);   /* PA3 = alternate function */

    /* Step 2: Set Alternate Function 7 (USART2) for PA2 and PA3 */
    /* AFRL register controls pins 0-7 */
    /* PA2 = bits 11:8, PA3 = bits 15:12 */
    GPIOA->AFR[0] &= ~(0xF << 8);   /* clear PA2 AF bits */
    GPIOA->AFR[0] |=  (0x7 << 8);   /* PA2 = AF7 (USART2) */

    GPIOA->AFR[0] &= ~(0xF << 12);  /* clear PA3 AF bits */
    GPIOA->AFR[0] |=  (0x7 << 12);  /* PA3 = AF7 (USART2) */

    /* Step 3: Configure USART2 */
    /* Baud rate = 16MHz / 115200 = 139 */
    USART2->BRR = 139;

    /* Step 4: Enable TX, RX, and RXNE interrupt in CR1 */
    /* Bit 2  = RE  (receiver enable)
     * Bit 3  = TE  (transmitter enable)
     * Bit 5  = RXNEIE (RX interrupt enable)
     * Bit 13 = UE  (UART enable) */
    USART2->CR1 = (1 << 2) | (1 << 3) | (1 << 5) | (1 << 13);

    /* Step 5: Enable USART2 interrupt in NVIC */
    NVIC_EnableIRQ(USART2_IRQn);

    /* Step 6: Initialize ring buffer */
    rb_init(&rx_buffer);
}

void uart_send_char(char c)
{
    /* Wait until TX buffer is empty (TXE bit = bit 7 in SR) */
    while (!(USART2->SR & (1 << 7)));
    USART2->DR = c;
}

void uart_send_string(const char *str)
{
    while (*str)
    {
        uart_send_char(*str);
        str++;
    }
}

int uart_read_char(char *c)
{
    return rb_read(&rx_buffer, c);
}

/* UART RX Interrupt handler — called when a character arrives */
void USART2_IRQHandler(void)
{
    /* Check RXNE flag (bit 5) — data ready to read */
    if (USART2->SR & (1 << 5))
    {
        char c = (char)USART2->DR;   /* reading DR clears the flag */
        rb_write(&rx_buffer, c);      /* store in ring buffer */
    }
}
