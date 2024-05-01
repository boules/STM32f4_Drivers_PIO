#ifndef SCHED_H
#define SCHED_H




#include "common_macros.h"
#include "platform_types.h"



/*****************defines******************/
#define MAX_SUPPORTED_TASKS 4

// Tick_Time is the shortest time per interrupt
#define TICK_TIME   4 //1s 0.5s



// for any configurations configurable by the user
typedef struct
{
	char *name;
	uint32 ofset_ms; // time until the task is first executed
	uint32 periodicity; //frquency of the task
	uint32 priority;
	PtrFun_t function;
	
}SCHED_InitStruct;



/****************************config**********************************/
// Res_t sched_registerrunnable(Runnable_Struct *runnable);
/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

// To enable timers
void SCHED_Init(void);
// all tasks must be created between the init and the start
// Although our implementation will allow tasks to be run after starting sched
void SCHED_start(void);


#endif