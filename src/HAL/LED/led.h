#ifndef LED_H
#define LED_H

#include "dio.h"
#include "port.h"

/**************Led Config*************/
#define LED_MASK(PortNum, PinNum)		(PortNum<<4 | PinNum)

/**************Led Config*************/
#define		LED1		LED_MASK(PORTA, PIN4)
#define		LED2		LED_MASK(PORTA, PIN5)
#define		LED3		LED_MASK(PORTA, PIN6)




void LED_on(uint8 ledId);
void LED_off(uint8 ledId);
void LED_toggle(uint8 ledId);



#endif 