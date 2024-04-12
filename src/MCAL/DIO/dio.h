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


void DIO_setupPinDirection(uint8 portNum, uint8 pinNum, GPIO_PinDirectionType direction );
void DIO_setupChannelDirection_offset(uint8 portNum, uint8 numOfPins, uint8 offset, GPIO_PinDirectionType direction);


/******************************************/
uint32 DIO_readChannel_offset(uint8 portNum, uint8 numOfPins, uint8 offset);
void DIO_writeChannel_offset(uint8 portNum, uint8 numOfPins, uint8 offset, uint32 value);

/*******************************************/
// uint32 DIO_readPort(uint8 portNum);

// void DIO_writePort(uint8 port_num, uint16 value);
// void DIO_writeHalfPort(uint8 port_num, uint8 value);
// uint8 DIO_readHalfPort(uint8 port_num);






#endif