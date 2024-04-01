#include "sched.h"
#include "platform_types.h"
#include "systick.h"






// For any internal configuration
typedef struct{
	Runnable_Struct *runnable_struct_ptr;
	uint32 remainTime_ms;//fadly ad eh w ttnafzzz

}SchedTracker_Struct;



SchedTracker_Struct schedTracker[MAX_SUPPORTED_TASKS];





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

static void sched(void)
{
	uint32_t i;
	for(i = 0; i < MAX_SUPPORTED_TASKS; i++)
	{
		// if task not initialized, calling cb will cause a crash as cb is null
		if(schedTracker[i].runnable_struct_ptr && schedTracker[i].runnable_struct_ptr->runnableFunction)
		{	
			

			if (schedTracker[i].remainTime_ms == 0){

				schedTracker[i].runnable_struct_ptr->runnableFunction();
				schedTracker[i].remainTime_ms = schedTracker[i].runnable_struct_ptr->periodicity;
			} 
			
			// The next schedular will read that state
			schedTracker[i].remainTime_ms -= TICK_TIME; // One ms passed
			
		}



	}
}
void sched_init( Runnable_Struct * runnablesArray)
{
	// init vars (if needed)
	



	// seting the runnable configurations
	for (uint32 i=0; i<MAX_SUPPORTED_TASKS; i++){
		schedTracker[i].runnable_struct_ptr = &runnablesArray[i];
		schedTracker[i].remainTime_ms = runnablesArray[i].ofset_ms;

	}



	// systick configure
	STK_setTime_ms (TICK_TIME);
	STK_setCallBack(tickcb);
	//we start systick at sched_state
	
}

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
Res_t sched_registerrunnable(Runnable_Struct* r)
{
	if(r && (schedTracker[r->priority].runnable == NULL))
	{
		schedTracker[r->priority].runnable = r;
		schedTracker[r->priority].remainTime_ms = r->delayMS;
	}		
}
*/
