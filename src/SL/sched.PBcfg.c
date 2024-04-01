

#include "sched.h"


extern void green_led();
extern void yellow_led();
extern void red_led();


const Runnable_Struct runnablesArray[MAX_SUPPORTED_TASKS] = {
	[0] = {
		.name = "green_runnable_1",
		.ofset_ms = 0,
		.periodicity = 1000,
		.priority = 0,
		.runnableFunction = &green_led
	},

	[1] = {
		.name = "yellow_runnable_2",
		.ofset_ms = 0000,
		.periodicity = 2000,
		.priority = 1,
		.runnableFunction = &yellow_led
	},

	[2] = {
		.name = "red_runnable_3",
		.ofset_ms = 0000,
		.periodicity = 4000,
		.priority = 1,
		.runnableFunction = &red_led
	}
};