#ifndef DIO_H
#define DIO_H

#include "port.h"




typedef enum
{
	PIN_INPUT,PIN_OUTPUT
}GPIO_PinDirectionType;





void DIO_setPin(uint8 portNum, uint8 pinNum);
void DIO_clearPin(uint8 portNum, uint8 pinNum);
void DIO_togglePin(uint8 portNum, uint8 pinNum);




/******************************************/
uint32 DIO_readChannel_ofset(uint8 portNum, uint8 numOfPins, uint8 ofset);
void DIO_writeChannel_ofset(uint8 portNum, uint8 numOfPins, uint8 ofset, uint32 value);

// void DIO_writePort(uint8 port_num, uint16 value);
// void DIO_writeHalfPort(uint8 port_num, uint8 value);
// uint8 DIO_readHalfPort(uint8 port_num);


void DIO_setupPinDirection(uint8 portNum, uint8 pinNum, GPIO_PinDirectionType direction );



#endif