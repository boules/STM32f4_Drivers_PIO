 /******************************************************************************
 *
 * Module: Os
 *
 * File Name: Os.c
 *
 * Description: Source file for Os Scheduler.
 *
 * Author: Mohamed Tarek
 ******************************************************************************/

#include "Os.h"
#include "App.h"
// #include "Button.h"
#include "Led.h"
// #include "Gpt.h"

#include "common_macros.h"
#include "systick.h"


/* Global variable store the Os Time */
static uint16 g_Time_Tick_Count = 0;

/* Global variable to indicate the the timer has a new tick */
static uint8 g_New_Time_Tick_Flag = 0;

/*********************************************************************************************/
void Os_start(void)
{
    /* Global Interrupts Enable */
    Enable_Interrupts();
    
    /* 
     * Set the Call Back function to call Os_NewTimerTick
     * this function will be called every SysTick Interrupt (20ms)
     */
    // SysTick_SetCallBack(Os_NewTimerTick);
    

    /* Start SysTickTimer to generate interrupt every 20ms */
    // SysTick_Start(OS_BASE_TIME);
	STK_startITms_SetCBF(OS_BASE_TIME, Os_NewTimerTick);


    /* Execute the Init Task */
    Init_Task();

    /* Start the Os Scheduler */
    Os_Scheduler();
}

/*********************************************************************************************/
void Os_NewTimerTick(void)
{
    /* Increment the Os time by OS_BASE_TIME */
    g_Time_Tick_Count   += OS_BASE_TIME;

    /* Set the flag to 1 to indicate that there is a new timer tick */
    g_New_Time_Tick_Flag = 1;
}

/*********************************************************************************************/
void Os_Scheduler(void)
{
    while(1)
    {
		/* Code is only executed in case there is a new timer tick */
		if(g_New_Time_Tick_Flag == 1)
		{
			switch(g_Time_Tick_Count)
			{
			case 20:
				Green_Led_Task();

				g_New_Time_Tick_Flag = 0;
				break;

			case 40:
				Green_Led_Task();
				Yellow_Led_Task();

				g_New_Time_Tick_Flag = 0;
				break;

			case 80:
				Green_Led_Task();
				Yellow_Led_Task();
				Red_Led_Task();

				g_New_Time_Tick_Flag = 0;
				g_Time_Tick_Count = 0;
				break;


			}



			/* ADDED BY ME FOR TESTING SOMETHING*/
			g_New_Time_Tick_Flag = 0;
		}
    }

}
/*********************************************************************************************/



#define OS_DEFUALT_DESIGN
#ifndef OS_DEFUALT_DESIGN
/* Code is only executed in case there is a new timer tick */
		if(g_New_Time_Tick_Flag == 1)
		{
			switch(g_Time_Tick_Count)
			{
			case 20:
			case 100:
				Button_Task();

				g_New_Time_Tick_Flag = 0;
				break;

			case 40:
			case 80:
				Button_Task();
				Led_Task();

				g_New_Time_Tick_Flag = 0;
				break;

			case 60:
				Button_Task();
				App_Task();

				g_New_Time_Tick_Flag = 0;
				break;

				
			case 120:
				Button_Task();
				App_Task();
				Led_Task();
				
				g_New_Time_Tick_Flag = 0;
				g_Time_Tick_Count = 0;
				break;
			}
		}
#endif
