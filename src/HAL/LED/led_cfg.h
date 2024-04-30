#ifndef LED_CFG_H
#define LED_CFG_H

#include "led.h"

/* User Defined LEDs */
/**use macro defined in led.h
 *LED_ID(PortNum, PinNum)
*/

/**
 * Enter your defined Leds here
*/
#define		GreenLED		LED_ID(PORTA, PIN4)
#define		YellowLED		LED_ID(PORTA, PIN5)
#define		RedLED			LED_ID(PORTA, PIN6)


#endif