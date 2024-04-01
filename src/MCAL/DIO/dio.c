#include "platform_types.h"
#include "dio.h"
#include "port_regs.h"
#include "port.h"
#include "common_macros.h"

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




/**
 * set Pin mode and direction
*/
void DIO_setupPinDirection(uint8 portNum, uint8 pinNum, GPIO_PinDirectionType direction ){
	
	volatile GPIO_RegStruct* GPIOx_basePtr = NULL_PTR;											/* Port Channel pointer */
	getBaseAddress(portNum, &GPIOx_basePtr);

	/* Configure the mode related to MODER  Register */
	CLEAR_MULTIBLE_BITS((GPIOx_basePtr->MODER), TWO_ONES_MASK, (pinNum), SHIFTBY_TWO);							/*!< Clear to SetbyOR */
	SETBYOR_MULTIBLE_BITS((GPIOx_basePtr->MODER), (direction), (pinNum), SHIFTBY_TWO);
}


void DIO_setupChannelDirection_ofset(uint8 portNum, uint8 numOfPins, uint8 ofset, GPIO_PinDirectionType direction){
	volatile GPIO_RegStruct* GPIOx_basePtr = NULL_PTR;											/* Port Channel pointer */
	getBaseAddress(portNum, &GPIOx_basePtr);

	// ((0x55555555<<(((16-numOfPins)*2)))>>(((16+ofset-numOfPins)*2)))
}



void DIO_writeChannel_ofset(uint8 portNum, uint8 numOfPins, uint8 ofset, uint32 value){
	
	volatile GPIO_RegStruct* GPIOx_basePtr = NULL_PTR;											/* Port Channel pointer */
	getBaseAddress(portNum, &GPIOx_basePtr);

	CLEARANDSET_MULTIBLE_BITS((GPIOx_basePtr->ODR), ofset, numOfPins, value);
}

uint32 DIO_readChannel_ofset(uint8 portNum, uint8 numOfPins, uint8 ofset){
	volatile GPIO_RegStruct* GPIOx_basePtr = NULL_PTR;											/* Port Channel pointer */
	getBaseAddress(portNum, &GPIOx_basePtr);

	return (((GPIOx_basePtr->IDR) & ( ((1<<numOfPins)-1)<<ofset))>> ofset);
}

uint32 DIO_readPort(uint8 portNum){
	volatile GPIO_RegStruct* GPIOx_basePtr = NULL_PTR;											/* Port Channel pointer */
	getBaseAddress(portNum, &GPIOx_basePtr);

	return (GPIOx_basePtr->IDR);
}
