
#ifndef PORT_REGS_H
#define PORT_REGS_H

/******************************************************************************
 *                       			Includes			                      *
 ******************************************************************************/
#ifndef PLATFORM_TYPES_H
#include "platform_types.h"
#endif




/*******************************************************************************
 *                       	GPIO Registers base addresses	                   *
 ******************************************************************************/
/********************  GPIO Registers base addresses  ********************/
#define GPIO_PORTA_BASE_ADDRESS           (volatile GPIO_RegStruct *)0x40020000
#define GPIO_PORTB_BASE_ADDRESS           (volatile GPIO_RegStruct *)0x40020400
#define GPIO_PORTC_BASE_ADDRESS           (volatile GPIO_RegStruct *)0x40020800
#define GPIO_PORTD_BASE_ADDRESS           (volatile GPIO_RegStruct *)0x40020C00
#define GPIO_PORTE_BASE_ADDRESS           (volatile GPIO_RegStruct *)0x40021000
#define GPIO_PORTF_BASE_ADDRESS           (volatile GPIO_RegStruct *)0x40021400
#define GPIO_PORTG_BASE_ADDRESS           (volatile GPIO_RegStruct *)0x40021800
#define GPIO_PORTH_BASE_ADDRESS           (volatile GPIO_RegStruct *)0x40021C00
#define GPIO_PORTI_BASE_ADDRESS           (volatile GPIO_RegStruct *)0x40022000


/*******************************************************************************
 *                       GPIO Registers Structure Mapping                      *
 ******************************************************************************/
typedef struct
{
	volatile uint32 MODER;		/*!< GPIO port mode register,               Address offset: 0x00	*/
	volatile uint32 OTYPER;		/*!< GPIO port output type register,        Address offset: 0x04	*/
	volatile uint32 OSPEEDR;	/*!< GPIO port output speed register,       Address offset: 0x08	*/
	volatile uint32 PUPDR;		/*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C	*/
	volatile uint32 IDR;		/*!< GPIO port input data register,         Address offset: 0x10	*/
	volatile uint32 ODR;		/*!< GPIO port output data register,        Address offset: 0x14	*/
	volatile uint16 BSRRL;		/*!< GPIO port bit set/reset low register,  Address offset: 0x18	*/ /*BSRR Devided to two regs H & L*/
	volatile uint16 BSRRH;		/*!< GPIO port bit set/reset high register, Address offset: 0x1A	*/
	volatile uint32 LCKR;		/*!< GPIO port configuration lock register, Address offset: 0x1C	*/
	volatile uint32 AFRL;		/*!< GPIO alternate function registers,     Address offset: 0x20	*/
	volatile uint32 AFRH;		/*!< GPIO alternate function registers,     Address offset: 0x24	*/
} GPIO_RegStruct;





#endif /* PORT_REGS_H */
