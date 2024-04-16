

#include "sched.h"
#include "led.h"
#include "led_cfg.h"

/* Write here your runnable functions */
void runnable_Function_1(){
	LED_toggle(GreenLED);
}

void runnable_Function_2(){
	LED_toggle(YellowLED);
}

void runnable_Function_3(){
	LED_toggle(RedLED);
}







/* Array Contains all runnables details */
const SCHED_InitStruct runnablesArray[MAX_SUPPORTED_TASKS] = {
	[0] = {
		.name = "green_runnable_1",
		.ofset_ms = 0,
		.periodicity = 1000,
		.priority = 0,
		.function = &runnable_Function_1
	},

	[1] = {
		.name = "yellow_runnable_2",
		.ofset_ms = 0000,
		.periodicity = 2000,
		.priority = 1,
		.function = &runnable_Function_2
	},

	[2] = {
		.name = "red_runnable_3",
		.ofset_ms = 0000,
		.periodicity = 4000,
		.priority = 1,
		.function = &runnable_Function_3
	}
};