#include <stdint.h>
#include "stm32f401xe.h"
#include "rcc.h"
#include "gpio.h"
#include "systick.h"
#include "scheduler.h"
#include "uart.h"
#include "cli.h"

void task_led(void)
{
    led_toggle();
}

void task_status(void)
{
    uart_send_string("STM32 is alive!\r\n");
}

void task_cli(void){
	cli_task();
}

int main(void)
{
    rcc_init();
    gpio_init();
    uart_init();
    systick_init();
    scheduler_init();

    scheduler_add_task(task_cli, 10);

    //Show welcome messages
    cli_init();

    while(1)
    {
        scheduler_run();
    }
}
