/*
 * scheduler.c
 *
 *  Created on: Mar 21, 2026
 *      Author: aravindhan-tv
 */
#include "scheduler.h"
#include "systick.h"

static task_t tasks[MAX_TASKS];
static uint8_t task_count = 0;

void scheduler_init(void){
	task_count=0;
}

void scheduler_add_task(void (*task)(void), uint32_t period_ms){
	if (task_count < MAX_TASKS){
		tasks[task_count].task	= task;
		tasks[task_count].period_ms = period_ms;
		tasks[task_count].last_run = 0;
		task_count++;
	}
}

void scheduler_run(void)
{
	uint8_t i;
	for(i=0; i< task_count; i++){
		uint32_t now = systick_get_tick();

		if ((now-tasks[i].last_run) >= tasks[i].period_ms){
			tasks[i].task();			//Run the task
			tasks[i].last_run = now;	//update the last run
		}
	}
}
