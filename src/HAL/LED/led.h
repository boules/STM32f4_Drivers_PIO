#ifndef LED_H
#define LED_H

#include "dio.h"
#include "port.h"
#include "led_cfg.h"

/* Led ID Mask */
#define LED_ID(PortNum, PinNum)		(PortNum<<4 | PinNum)


/* Functions Declerations */
void LED_on(uint8 ledId);
void LED_off(uint8 ledId);
void LED_toggle(uint8 ledId);



#endif 