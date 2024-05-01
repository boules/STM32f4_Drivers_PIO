#include "sched.h"
#include "platform_types.h"
#include "systick.h"






// For any internal configuration
// Single Task schedular Data
typedef struct{
	const SCHED_InitStruct *Task;
	uint32 remainTime_ms;//fadly ad eh w ttnafzzz

}SCHED_TrackerStruct;


extern const SCHED_InitStruct runnablesSet [MAX_SUPPORTED_TASKS];
SCHED_TrackerStruct taskManager[MAX_SUPPORTED_TASKS];





// variable to determie if we should call sched
static uint32_t g_pendingTicks;
void SCHED_newTick(void)
{
	g_pendingTicks++;

}


//This kernel like
static void SCHED_core(void){
	
	uint32_t i;
	for(i = 0; i < MAX_SUPPORTED_TASKS; i++){
		// if task not initialized, calling cb will cause a crash as cb is null
		if( (taskManager[i].Task != NULL_PTR) && (taskManager[i].Task->function !=  NULL_PTR) )
		{	
			

			if (taskManager[i].remainTime_ms == 0){ /* (<=) 0*/

				taskManager[i].Task->function();
				taskManager[i].remainTime_ms = taskManager[i].Task->periodicity;
			} 
			
			// The next schedular will read that state
			taskManager[i].remainTime_ms -= TICK_TIME; // One ms passed
			
		}



	}
}

/* Setting and intializing all Sched component*/
void SCHED_Init(void)
{
	// seting the runnable configurations
	for (uint32 i=0; i<MAX_SUPPORTED_TASKS; i++){
		taskManager[i].Task = &runnablesSet[i];
		taskManager[i].remainTime_ms = runnablesSet[i].ofset_ms;

	}

	// systick configure
	STK_setIT_time_CBF(TICK_TIME, SCHED_newTick);
}

/* Strat Sched Ticking */
void SCHED_start(void)
{
	Enable_Interrupts();
	// Disable_Interrupts();

	/* Drivers Init Task */
	Init_Task();

	/* Start sched timing and interrupts */
	STK_start();
	
	while(1)
	{
		// if g_pendingTicks is greater than one, it means
		// that cpu load is over 100%, because two syscalls
		// occurred when sched was already running
		if(g_pendingTicks)
		{
			g_pendingTicks--;
			SCHED_core();
		}
		
	}
}






/*
Res_t sched_registerrunnable(SCHED_InitStruct* r)
{
	if(r && (taskManager[r->priority].runnable == NULL))
	{
		taskManager[r->priority].runnable = r;
		taskManager[r->priority].remainTime_ms = r->delayMS;
	}		
}
*/
