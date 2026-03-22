#include "cli.h"
#include "uart.h"
#include "gpio.h"
#include "systick.h"
#include <string.h>

/* Buffer to build the command string */
#define CMD_MAX_LEN  64

static char cmd_buffer[CMD_MAX_LEN];
static uint8_t cmd_index = 0;

void cli_init(void)
{
    cmd_index = 0;
    uart_send_string("\r\n=== Bare Metal CLI ===\r\n");
    uart_send_string("Type 'help' to see commands\r\n");
    uart_send_string("> ");
}

/* Parse and execute the command */
static void cli_process(char *input)
{
    /* Split input into command and argument */
    char *cmd = strtok(input, " ");
    char *arg = strtok(NULL,  " ");

    if (cmd == NULL) return;   /* empty command */

    /* ── help ── */
    if (strcmp(cmd, "help") == 0)
    {
        uart_send_string("\r\nAvailable commands:\r\n");
        uart_send_string("  help        - show this message\r\n");
        uart_send_string("  led on      - turn LED on\r\n");
        uart_send_string("  led off     - turn LED off\r\n");
        uart_send_string("  status      - show system status\r\n");
    }

    /* ── led ── */
    else if (strcmp(cmd, "led") == 0)
    {
        if (arg == NULL)
        {
            uart_send_string("Usage: led on | led off\r\n");
        }
        else if (strcmp(arg, "on") == 0)
        {
            led_on();
            uart_send_string("LED turned ON\r\n");
        }
        else if (strcmp(arg, "off") == 0)
        {
            led_off();
            uart_send_string("LED turned OFF\r\n");
        }
        else
        {
            uart_send_string("Unknown argument. Use: led on | led off\r\n");
        }
    }

    /* ── status ── */
    else if (strcmp(cmd, "status") == 0)
    {
        uint32_t t = systick_get_tick();
        uint32_t seconds = t / 1000;
        uint32_t minutes = seconds / 60;
        seconds = seconds % 60;

        char buf[64];
        uart_send_string("--- System Status ---\r\n");

        /* Print uptime manually without printf */
        uart_send_string("Uptime : ");
        buf[0] = '0' + (minutes / 10);
        buf[1] = '0' + (minutes % 10);
        buf[2] = ':';
        buf[3] = '0' + (seconds / 10);
        buf[4] = '0' + (seconds % 10);
        buf[5] = '\r';
        buf[6] = '\n';
        buf[7] = '\0';
        uart_send_string(buf);

        uart_send_string("Scheduler: running\r\n");
    }

    /* ── unknown ── */
    else
    {
        uart_send_string("Unknown command. Type 'help'\r\n");
    }
}

/* CLI task — called every 10ms by scheduler */
void cli_task(void)
{
    char c;

    /* Read all available characters from ring buffer */
    while (uart_read_char(&c))
    {
        if (c == '\r' || c == '\n')
        {
            /* Enter pressed — process the command */
            uart_send_string("\r\n");

            if (cmd_index > 0)
            {
                cmd_buffer[cmd_index] = '\0';   /* null terminate */
                cli_process(cmd_buffer);         /* execute command */
                cmd_index = 0;                   /* reset buffer    */
            }

            uart_send_string("> ");   /* show prompt again */
        }
        else if (c == 127 || c == '\b')
        {
            /* Backspace */
            if (cmd_index > 0)
            {
                cmd_index--;
                uart_send_string("\b \b");   /* erase character on screen */
            }
        }
        else if (cmd_index < CMD_MAX_LEN - 1)
        {
            /* Normal character — add to buffer and echo it */
            cmd_buffer[cmd_index++] = c;
            uart_send_char(c);   /* echo back so you see what you typed */
        }
    }
}
