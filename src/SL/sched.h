#ifndef SCHED_H
#define SCHED_H




#include "common_macros.h"
#include "platform_types.h"



/*****************defines******************/
#define MAX_SUPPORTED_TASKS 3






/****************typedefs******************/
typedef void (*FunPtr_t) (void);

// for any configurations configurable by the user
typedef struct
{
	char *name;
	uint32 ofset_ms; // time until the task is first executed
	uint32 periodicity; //frquency of the task
	uint32 priority;
	FunPtr_t function;
	
}SCHED_InitStruct;



/****************************config**********************************/















// Res_t sched_registerrunnable(Runnable_Struct *runnable);
/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

// To enable timers
void sched_init( SCHED_InitStruct * runnablesArray);
// all tasks must be created between the init and the start
// Although our implementation will allow tasks to be run after starting sched
void sched_start(void);


#endif