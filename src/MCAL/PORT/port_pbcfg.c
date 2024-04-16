
/******************************************************************************
 *                       			Includes			                      *
 ******************************************************************************/
#include "port.h"
#include "port_cfg.h"
#include "std_types.h"




/******************************************************************************
 *                       		configure pins			                      *
 ******************************************************************************/
/*
* 	1.port							PORTA, PORTB, PORTC,..
* 	2.pin							PIN0, PIN1, ..PIN15
*
* 	3.mode;							GPIO_Mode_IN=0x00, GPIO_Mode_OUT=0x01, GPIO_Mode_AF=0x02, GPIO_Mode_AN=0x03 
* 	4.mode_changeability;			CHANGEABLE	=0x00, UNCHANGEABLE	=0x01
*
* 	5.direction;					PORT_PIN_IN	=0x00, PORT_PIN_OUT	=0x01
* 	6.direction_changeability;		CHANGEABLE	=0x00, UNCHANGEABLE	=0x01
*
* 	7.GPIO_PuPd;					No_Pull		=0x00, Pull_Up		=0x01, Pull_Down=0x02
* 	8.GPIO_OType;					OType_PP	=0x00, OType_OD		=0x01
*
* 	9.initial_value;				STD_LOW		=0x00, STD_HIGH		=0x01
* 	10.GPIO_Speed;					GPIO_Speed_2MHz=0x00, GPIO_Speed_25MHz, GPIO_Speed_50MHz, GPIO_Speed_100MHz













* please note that PA13, PA14, PA15  are the JTAG pins. Be careful editing them.
*/
Port_ConfigPinStruct Port_pinConfigurationSet[configuredPins]={
	{.port = PORTA, .pin= PIN0, .mode = GPIO_Mode_OUT, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= DEFAULT_FEATURE, .GPIO_OType= DEFAULT_FEATURE, .initial_value= STD_HIGH, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= DEFAULT_FEATURE},
	{.port = PORTA, .pin= PIN2, .mode = GPIO_Mode_OUT, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= DEFAULT_FEATURE, .GPIO_OType= DEFAULT_FEATURE, .initial_value= STD_HIGH, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= DEFAULT_FEATURE},
	{.port = PORTA, .pin= PIN3, .mode = GPIO_Mode_OUT, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= DEFAULT_FEATURE, .GPIO_OType= DEFAULT_FEATURE, .initial_value= STD_HIGH, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= DEFAULT_FEATURE},
	
//USART
	{.port = PORTA, .pin= PIN9, .mode = GPIO_Mode_AF, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= DEFAULT_FEATURE, .GPIO_OType= DEFAULT_FEATURE, .initial_value= STD_LOW, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= PORT_ALTERNATE_FUNCTION_07_MODE},
	{.port = PORTA, .pin= PIN10, .mode = GPIO_Mode_AF, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= DEFAULT_FEATURE, .GPIO_OType= DEFAULT_FEATURE, .initial_value= STD_LOW, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= PORT_ALTERNATE_FUNCTION_07_MODE},
	
//I2C
	{.port = PORTB, .pin= PIN7, .mode = GPIO_Mode_AF, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= DEFAULT_FEATURE, .GPIO_OType= 1, .initial_value= STD_LOW, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= PORT_ALTERNATE_FUNCTION_04_MODE},
	{.port = PORTB, .pin= PIN6, .mode = GPIO_Mode_AF, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= DEFAULT_FEATURE, .GPIO_OType= 1, .initial_value= STD_LOW, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= PORT_ALTERNATE_FUNCTION_04_MODE},

//sched_leds
	{.port = PORTA, .pin= PIN4, .mode = GPIO_Mode_OUT, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= DEFAULT_FEATURE, .GPIO_OType= DEFAULT_FEATURE, .initial_value= STD_HIGH, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= DEFAULT_FEATURE},
	{.port = PORTA, .pin= PIN5, .mode = GPIO_Mode_OUT, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= DEFAULT_FEATURE, .GPIO_OType= DEFAULT_FEATURE, .initial_value= STD_HIGH, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= DEFAULT_FEATURE},
	{.port = PORTA, .pin= PIN6, .mode = GPIO_Mode_OUT, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= DEFAULT_FEATURE, .GPIO_OType= DEFAULT_FEATURE, .initial_value= STD_HIGH, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= DEFAULT_FEATURE},

//LCD
	{.port = PORTB, .pin= PIN13, .mode = GPIO_Mode_OUT, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= DEFAULT_FEATURE, .GPIO_OType= DEFAULT_FEATURE, .initial_value= STD_LOW, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= DEFAULT_FEATURE},
	{.port = PORTB, .pin= PIN14, .mode = GPIO_Mode_OUT, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= DEFAULT_FEATURE, .GPIO_OType= DEFAULT_FEATURE, .initial_value= STD_LOW, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= DEFAULT_FEATURE},
	{.port = PORTB, .pin= PIN15, .mode = GPIO_Mode_OUT, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= DEFAULT_FEATURE, .GPIO_OType= DEFAULT_FEATURE, .initial_value= STD_LOW, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= DEFAULT_FEATURE},

	{.port = PORTC, .pin= PIN0, .mode = GPIO_Mode_OUT, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= DEFAULT_FEATURE, .GPIO_OType= DEFAULT_FEATURE, .initial_value= STD_LOW, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= DEFAULT_FEATURE},
	{.port = PORTC, .pin= PIN1, .mode = GPIO_Mode_OUT, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= DEFAULT_FEATURE, .GPIO_OType= DEFAULT_FEATURE, .initial_value= STD_LOW, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= DEFAULT_FEATURE},
	{.port = PORTC, .pin= PIN2, .mode = GPIO_Mode_OUT, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= DEFAULT_FEATURE, .GPIO_OType= DEFAULT_FEATURE, .initial_value= STD_LOW, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= DEFAULT_FEATURE},
	{.port = PORTC, .pin= PIN3, .mode = GPIO_Mode_OUT, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= DEFAULT_FEATURE, .GPIO_OType= DEFAULT_FEATURE, .initial_value= STD_LOW, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= DEFAULT_FEATURE},
	{.port = PORTC, .pin= PIN4, .mode = GPIO_Mode_OUT, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= DEFAULT_FEATURE, .GPIO_OType= DEFAULT_FEATURE, .initial_value= STD_LOW, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= DEFAULT_FEATURE},
	{.port = PORTC, .pin= PIN5, .mode = GPIO_Mode_OUT, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= DEFAULT_FEATURE, .GPIO_OType= DEFAULT_FEATURE, .initial_value= STD_LOW, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= DEFAULT_FEATURE},
	{.port = PORTC, .pin= PIN6, .mode = GPIO_Mode_OUT, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= DEFAULT_FEATURE, .GPIO_OType= DEFAULT_FEATURE, .initial_value= STD_LOW, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= DEFAULT_FEATURE},
	{.port = PORTC, .pin= PIN7, .mode = GPIO_Mode_OUT, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= DEFAULT_FEATURE, .GPIO_OType= DEFAULT_FEATURE, .initial_value= STD_LOW, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= DEFAULT_FEATURE},

//Keypad
/*rows*/
	{.port = PORTB, .pin= PIN0, .mode = GPIO_Mode_IN, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= Pull_Down, .GPIO_OType= DEFAULT_FEATURE, .initial_value= STD_LOW, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= DEFAULT_FEATURE},
	{.port = PORTB, .pin= PIN1, .mode = GPIO_Mode_IN, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= Pull_Down, .GPIO_OType= DEFAULT_FEATURE, .initial_value= STD_LOW, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= DEFAULT_FEATURE},
	{.port = PORTB, .pin= PIN2, .mode = GPIO_Mode_IN, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= Pull_Down, .GPIO_OType= DEFAULT_FEATURE, .initial_value= STD_LOW, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= DEFAULT_FEATURE},
	{.port = PORTB, .pin= PIN3, .mode = GPIO_Mode_IN, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= Pull_Down, .GPIO_OType= DEFAULT_FEATURE, .initial_value= STD_LOW, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= DEFAULT_FEATURE},
/*cols*/
	{.port = PORTB, .pin= PIN4, .mode = GPIO_Mode_IN, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= DEFAULT_FEATURE, .GPIO_OType= DEFAULT_FEATURE, .initial_value= STD_LOW, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= DEFAULT_FEATURE},
	{.port = PORTB, .pin= PIN5, .mode = GPIO_Mode_IN, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= DEFAULT_FEATURE, .GPIO_OType= DEFAULT_FEATURE, .initial_value= STD_LOW, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= DEFAULT_FEATURE},
	{.port = PORTB, .pin= PIN6, .mode = GPIO_Mode_IN, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= DEFAULT_FEATURE, .GPIO_OType= DEFAULT_FEATURE, .initial_value= STD_LOW, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= DEFAULT_FEATURE},
	{.port = PORTB, .pin= PIN7, .mode = GPIO_Mode_IN, .mode_changeability= DEFAULT_FEATURE, .direction= DEFAULT_FEATURE, .direction_changeability= DEFAULT_FEATURE, .GPIO_PuPd= DEFAULT_FEATURE, .GPIO_OType= DEFAULT_FEATURE, .initial_value= STD_LOW, .GPIO_Speed= DEFAULT_FEATURE, .AlternateFunction= DEFAULT_FEATURE}

};
