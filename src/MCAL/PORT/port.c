
#include "port.h"
#include "port_regs.h"
#include "common_macros.h"
#include "std_types.h"
#include "platform_types.h"
#include "rcc.h"





/*
* there is 2 pointers configPtr points to config struct   mode, ..
* 					  GPIOx_basePtr pints to the want to config REG

* 1. mode 				00: Input (reset state)
* 						01: General purpose output mode
* 						10: Alternate function mode
* 						11: Analog mode
*


* editing 3 registers
* 1. MODER:mode
* 2. BSRR :set and rest (intial value) atomic access (just write operation)
* 3. AFR  :alternate fumction options
*
*/
void Port_Init(Port_ConfigPinStruct configArr[]){
	uint8 numOfLoops = configuredPins;
	volatile GPIO_RegStruct* GPIOx_basePtr = NULL_PTR;											/* Port Channel pointer */
	
	for (int i =0; i<numOfLoops; i++){
		Port_ConfigPinStruct* configPtr = &configArr[i];

		switch (configPtr->port){
			case PORTA:
				GPIOx_basePtr = (volatile GPIO_RegStruct *)GPIO_PORTA_BASE_ADDRESS;					/* PORTA Base Address */
				RCC_Enable(RCC_GPIOA);
				break;
			case PORTB:
				GPIOx_basePtr = (volatile GPIO_RegStruct *)GPIO_PORTB_BASE_ADDRESS;					/* PORTB Base Address */
				RCC_Enable(RCC_GPIOB);
				break;
			
			case PORTC:
				GPIOx_basePtr = (volatile GPIO_RegStruct *)GPIO_PORTC_BASE_ADDRESS;					/* PORTC Base Address */
				RCC_Enable(RCC_GPIOC);
				break;
			
			case PORTD:
				GPIOx_basePtr = (volatile GPIO_RegStruct *)GPIO_PORTD_BASE_ADDRESS;					/* PORTD Base Address */
				RCC_Enable(RCC_GPIOD);
				break;
			
			case PORTH:
				GPIOx_basePtr = (volatile GPIO_RegStruct *)GPIO_PORTH_BASE_ADDRESS;					/* PORTH Base Address */
				RCC_Enable(RCC_GPIOH);
				break;
			
		}


		/* Preparing the alternate function before setting the MODER reg*/
		if(configPtr->mode == GPIO_Mode_AF){																			/*!< Alternate Fn modes*/
			
			if( configPtr->pin <= PIN7){
				CLEAR_MULTIBLE_BITS(GPIOx_basePtr->AFRL, FOUR_ONES_MASK, (configPtr->pin), SHIFTBY_FOUR);						/*!< Clear to SetbyOR */
				SETBYOR_MULTIBLE_BITS(GPIOx_basePtr->AFRL, (configPtr->AlternateFunction), (configPtr->pin), SHIFTBY_FOUR);		/*!< Set new configurations in MODE Reg for Pin as GPIO OUT */
			}else{
			
				CLEAR_MULTIBLE_BITS((GPIOx_basePtr->AFRH), FOUR_ONES_MASK, ((configPtr->pin)-8), SHIFTBY_FOUR);						/*!< Clear to SetbyOR */
				SETBYOR_MULTIBLE_BITS(GPIOx_basePtr->AFRH, (configPtr->AlternateFunction), ((configPtr->pin)-8), SHIFTBY_FOUR);		/*!< Set new configurations in MODE Reg for Pin as GPIO OUT */
			}
		}

		/* Configure the mode related to MODER  Register */
		CLEAR_MULTIBLE_BITS((GPIOx_basePtr->MODER), TWO_ONES_MASK, (configPtr->pin), SHIFTBY_TWO);							/*!< Clear to SetbyOR */
		SETBYOR_MULTIBLE_BITS((GPIOx_basePtr->MODER), (configPtr->mode), (configPtr->pin), SHIFTBY_TWO);					/*!< Set new configurations in MODE Reg for Pin as GPIO OUT */
		
		if(configPtr->mode == GPIO_Mode_OUT) {
			/* Seting and resting the output pin*/
			if(configPtr->initial_value == STD_HIGH){
					SET_BIT((GPIOx_basePtr->BSRRL), (configPtr->pin));											/*!< At initial value Low setBit at Reset_Register(BSRRH) to Reset notice that this is an atomic access you just wrote in it.    instead of the read-write-modify operation */
			} else {
					SET_BIT((GPIOx_basePtr->BSRRH), (configPtr->pin));											/*!< At initial value High setBit at set_Register(BSRRL) to Set */
			}
		}

		/* Configure the Output Type related to OTYPER Register */
		CLEAR_BIT((GPIOx_basePtr->OTYPER), (configPtr->pin));
		SETBYOR_MULTIBLE_BITS((GPIOx_basePtr->OTYPER), (configPtr->GPIO_OType), (configPtr->pin), SHIFTBY_ONE);

		/* Configure the type of speed related to OSPEEDR Register */
		CLEAR_MULTIBLE_BITS((GPIOx_basePtr->OSPEEDR), TWO_ONES_MASK, (configPtr->pin), SHIFTBY_TWO);									/*!< Clear to SetbyOR */
		SETBYOR_MULTIBLE_BITS((GPIOx_basePtr->OSPEEDR), (configPtr->GPIO_Speed), (configPtr->pin), SHIFTBY_TWO);					/*!< Set new configurations in MODE Reg for Pin as GPIO OUT */

		/* Configure the Pullup or pulldown resistance related to PUPDR Register */
		CLEAR_MULTIBLE_BITS((GPIOx_basePtr->PUPDR), TWO_ONES_MASK, (configPtr->pin), SHIFTBY_TWO);									/*!< Clear to SetbyOR */
		SETBYOR_MULTIBLE_BITS((GPIOx_basePtr->PUPDR), (configPtr->GPIO_PuPd), (configPtr->pin), SHIFTBY_TWO);					/*!< Set new configurations in MODE Reg for Pin as GPIO OUT */


	}

	/* End Mode configurations */


}


