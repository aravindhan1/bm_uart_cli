/*
 * scheduler.h
 *
 *  Created on: Mar 21, 2026
 *      Author: aravindhan-tv
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <stdint.h>

//Maximun number of tasks
#define MAX_TASKS 10

typedef struct{
	void (*task)(void); //Function to call
	uint32_t period_ms; //How often in ms
	uint32_t last_run; //Last run time
}task_t ;

void scheduler_init (void);
void scheduler_add_task(void (*task)(void), uint32_t period_ms);
void scheduler_run(void);



#endif /* SCHEDULER_H_ */
