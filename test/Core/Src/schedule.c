/*
 * schedule.c
 *
 *  Created on: Nov 25, 2022
 *      Author: huynh
 */

#include "scheduler.h"

sTasks SCH_tasks_G[SCH_MAX_TASKS];
uint8_t current_index_task = 0;

void SCH_Init(void){
	for(uint32_t i = 0; i < SCH_MAX_TASKS; i++) {
		SCH_Delete_Task(i);
	}
	current_index_task = 0;
}

void SCH_Add_Task(void (* pFunction)(), uint32_t DELAY,  uint32_t PERIOD)
{
	if(current_index_task < SCH_MAX_TASKS){
		SCH_tasks_G[current_index_task].pTask = pFunction;
		SCH_tasks_G[current_index_task].Delay = DELAY;
		SCH_tasks_G[current_index_task].Period = PERIOD;
		SCH_tasks_G[current_index_task].RunMe = 0;
		SCH_tasks_G[current_index_task].TaskID = current_index_task;

		current_index_task++;
	}
}

void SCH_Update(void){
	for (int i = 0; i < current_index_task; i++) {
		if(SCH_tasks_G[i].Delay > 0){
			SCH_tasks_G[i].Delay--;
		}
		else{
			if(SCH_tasks_G[i].Period == 0){
				(*SCH_tasks_G[i].pTask)();
				i--;
				current_index_task--;
			}
			else {
			SCH_tasks_G[i].Delay = SCH_tasks_G[i].Period;
			SCH_tasks_G[i].RunMe =+1;
			}
		}
	}
}

void SCH_Dispatch_Tasks(void){
	for (int i = 0; i < current_index_task; i++){
		if(SCH_tasks_G[i].RunMe > 0){
			SCH_tasks_G[i].RunMe--;
			(*SCH_tasks_G[i].pTask)();
		}
	}
}

void SCH_Delete_Task(uint32_t ID){
	if(SCH_tasks_G[current_index_task].TaskID == ID){
		SCH_tasks_G[current_index_task].pTask = 0x0000;
		SCH_tasks_G[current_index_task].Delay = 0;
		SCH_tasks_G[current_index_task].Period = 0;
		SCH_tasks_G[current_index_task].RunMe = 0;
		SCH_tasks_G[current_index_task].TaskID = -1;
	}
}
