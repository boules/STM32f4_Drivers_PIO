

#include "sched.h"
#include "led.h"
#include "led_cfg.h"
#include "App.h"




/* Array Contains all runnables details */
const SCHED_InitStruct runnablesSet [MAX_SUPPORTED_TASKS] = {
	[0] = {
		.name = "green_runnable_1",
		.ofset_ms = 0,
		.periodicity = 8000,
		.priority = 0,
		.function = &Green_Led_Task
	},

	[1] = {
		.name = "yellow_runnable_2",
		.ofset_ms = 0000,
		.periodicity = 16000,
		.priority = 0,
		.function = &Yellow_Led_Task
	},

	[2] = {
		.name = "red_runnable_3",
		.ofset_ms = 0000,
		.periodicity = 32000,
		.priority = 0,
		.function = &Red_Led_Task
	},

	[3] = {
		.name = "LCD_Main_Task",
		.ofset_ms = 0000,
		.periodicity = 4,
		.priority = 0,
		.function = &LCD_Main_Task
	},

};