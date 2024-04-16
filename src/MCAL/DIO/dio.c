#include "platform_types.h"
#include "dio.h"
#include "port_regs.h"
#include "port.h"
#include "common_macros.h"

/** Functions Notes
 * All reads are read inputs 
 * All writes are write to output
 * 
 * if the direction is output... then the ODR pin is added to IDR pin too after cycle
**/

static inline void getBaseAddress(uint8 portNum, volatile GPIO_RegStruct** ptr){
	switch (portNum){
		case PORTA:
			*ptr = (volatile GPIO_RegStruct *)GPIO_PORTA_BASE_ADDRESS;					/* PORTA Base Address */
			break;

		case PORTB:
			*ptr = (volatile GPIO_RegStruct *)GPIO_PORTB_BASE_ADDRESS;					/* PORTB Base Address */
			break;

		case PORTC:
			*ptr = (volatile GPIO_RegStruct *)GPIO_PORTC_BASE_ADDRESS;					/* PORTC Base Address */
			break;

		case PORTD:
			*ptr = (volatile GPIO_RegStruct *)GPIO_PORTD_BASE_ADDRESS;					/* PORTD Base Address */
			break;

		case PORTH:
			*ptr = (volatile GPIO_RegStruct *)GPIO_PORTH_BASE_ADDRESS;					/* PORTH Base Address */
			break;
		
	}

}



void DIO_setPin(uint8 portNum, uint8 pinNum){
	volatile GPIO_RegStruct* GPIOx_basePtr = NULL_PTR;											/* Port Channel pointer */
	getBaseAddress(portNum, &GPIOx_basePtr);

	SET_BIT( ( GPIOx_basePtr->BSRRL ), pinNum );
}

void DIO_clearPin(uint8 portNum, uint8 pinNum){
	volatile GPIO_RegStruct* GPIOx_basePtr = NULL_PTR;											/* Port Channel pointer */
	getBaseAddress(portNum, &GPIOx_basePtr);

	SET_BIT( GPIOx_basePtr->BSRRH, pinNum );
}

void DIO_togglePin(uint8 portNum, uint8 pinNum){
	volatile GPIO_RegStruct* GPIOx_basePtr = NULL_PTR;											/* Port Channel pointer */
	getBaseAddress(portNum, &GPIOx_basePtr);

	TOGGLE_BIT(GPIOx_basePtr->ODR, pinNum);
}

/* read input Pin */
uint8 DIO_readPin(uint8 portNum, uint8 pinNum){
	volatile GPIO_RegStruct* GPIOx_basePtr = NULL_PTR;											/* Port Channel pointer */
	getBaseAddress(portNum, &GPIOx_basePtr);

	// return BIT_IS_SET(GPIOx_basePtr->IDR, pinNum);
	return ((GPIOx_basePtr->IDR >> pinNum) & 0b001);
}



/*-------------------------- Direction Handling ----------------------------*/
/* Change Single Pin direction */
void DIO_setupPinDirection(uint8 portNum, uint8 pinNum, GPIO_PinDirectionType direction ){
	volatile GPIO_RegStruct* GPIOx_basePtr = NULL_PTR;											/* Port Channel pointer */
	getBaseAddress(portNum, &GPIOx_basePtr);

	/* Configure the mode related to MODER  Register */
	CLEAR_MULTIBLE_BITS((GPIOx_basePtr->MODER), TWO_ONES_MASK, (pinNum), SHIFTBY_TWO);							/*!< Clear to SetbyOR */
	SETBYOR_MULTIBLE_BITS((GPIOx_basePtr->MODER), (direction), (pinNum), SHIFTBY_TWO);
}

/* Change connected group of Pins directions */
void DIO_setupChannelDirection_offset(uint8 portNum, uint8 numOfPins, uint8 offset, GPIO_PinDirectionType direction){
	volatile GPIO_RegStruct* GPIOx_basePtr = NULL_PTR;											/* Port Channel pointer */
	getBaseAddress(portNum, &GPIOx_basePtr);

	/* Clear the Mode Reg to set its direction value*/
	(GPIOx_basePtr->MODER) &=  ~(((1<<(numOfPins*2))-1)<<(offset*2));

	if (direction == PIN_OUTPUT){
		// x05 is multible 0101 as output is 01 and by overflow the reg the the unwanted bits will be zeros
		(GPIOx_basePtr->MODER) |= ((0x55555555>>(32-(numOfPins*2)))<<(offset*2));
	}else{
		// write zeros which will leave it clear
	}

}


/*---------------------------- Group setups -----------------------------*/
/* write connected group of pins whether HIGH or LOW*/
void DIO_writeChannel_offset(uint8 portNum, uint8 numOfPins, uint8 offset, uint32 value){
	
	volatile GPIO_RegStruct* GPIOx_basePtr = NULL_PTR;											/* Port Channel pointer */
	getBaseAddress(portNum, &GPIOx_basePtr);

	CLEARANDSET_MULTIBLE_BITS((GPIOx_basePtr->ODR), offset, numOfPins, value);
}

/* read connected group of pins Digital DATA*/
uint32 DIO_readChannel_offset(uint8 portNum, uint8 numOfPins, uint8 offset){
	volatile GPIO_RegStruct* GPIOx_basePtr = NULL_PTR;											/* Port Channel pointer */
	getBaseAddress(portNum, &GPIOx_basePtr);

	return (((GPIOx_basePtr->IDR) & ( ((1<<numOfPins)-1)<<offset))>> offset);
}


/* Reads all port input pins (16pins) of the specific port */
uint32 DIO_readPort(uint8 portNum){
	volatile GPIO_RegStruct* GPIOx_basePtr = NULL_PTR;											/* Port Channel pointer */
	getBaseAddress(portNum, &GPIOx_basePtr);

	return (GPIOx_basePtr->IDR);
}
