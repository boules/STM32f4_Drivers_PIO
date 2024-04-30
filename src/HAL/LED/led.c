#include "dio.h"
#include "platform_types.h"
#include "led.h"



void LED_on(uint8 ledId){
	DIO_setPin((ledId>>4), (ledId&0x0f));
}


void LED_off(uint8 ledId){
	DIO_clearPin((ledId>>4), (ledId&0x0f));
}


void LED_toggle(uint8 ledId){
	DIO_togglePin((ledId>>4), (ledId&0x0f));
}