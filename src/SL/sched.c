#include "sched.h"
#include "platform_types.h"
#include "systick.h"






// For any internal configuration
// Single Task schedular Data
typedef struct{
	SCHED_InitStruct *Task;
	uint32 remainTime_ms;//fadly ad eh w ttnafzzz

}SCHED_TrackerStruct;



SCHED_TrackerStruct taskxTracker[MAX_SUPPORTED_TASKS];





// Tick_Time is the shortest time per interrupt
#define TICK_TIME   1000 //1s 


// variable to determie if we should call sched
static uint32_t g_pendingTicks;
void tickcb(void)
{
	g_pendingTicks++;

	//clear count flag
	// STK_clearCountFlag();  الانتربت مش فارق معاه الفلاج
}


//This kernel like
static void sched(void){
	
	uint32_t i;
	for(i = 0; i < MAX_SUPPORTED_TASKS; i++){
		// if task not initialized, calling cb will cause a crash as cb is null
		if( (taskxTracker[i].Task != NULL_PTR) && (taskxTracker[i].Task->function !=  NULL_PTR) )
		{	
			

			if (taskxTracker[i].remainTime_ms == 0){

				taskxTracker[i].Task->function();
				taskxTracker[i].remainTime_ms = taskxTracker[i].Task->periodicity;
			} 
			
			// The next schedular will read that state
			taskxTracker[i].remainTime_ms -= TICK_TIME; // One ms passed
			
		}



	}
}


/* Setting and intializing all Sched component*/
void sched_init( SCHED_InitStruct * runnablesArray)
{
	// init vars (if needed)
	



	// seting the runnable configurations
	for (uint32 i=0; i<MAX_SUPPORTED_TASKS; i++){
		taskxTracker[i].Task = &runnablesArray[i];
		taskxTracker[i].remainTime_ms = runnablesArray[i].ofset_ms;

	}



	// systick configure
	STK_setTime_ms (TICK_TIME);
	STK_setCallBack(tickcb);
	//we start systick at sched_state
	
}

/* Strat Sched Ticking */
void sched_start(void)
{
	Enable_Interrupts();
	// Disable_Interrupts();
	STK_start();
	while(1)
	{
		// if g_pendingTicks is greater than one, it means
		// that cpu load is over 100%, because two syscalls
		// occurred when sched was already running
		if(g_pendingTicks)
		{
			g_pendingTicks--;
			sched();
		}
		// if(STK_isExpire())
		// {
		// 	// g_pendingTicks--;
		// 	sched();
		// }
		
	}
}






/*
Res_t sched_registerrunnable(SCHED_InitStruct* r)
{
	if(r && (taskxTracker[r->priority].runnable == NULL))
	{
		taskxTracker[r->priority].runnable = r;
		taskxTracker[r->priority].remainTime_ms = r->delayMS;
	}		
}
*/
