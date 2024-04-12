/******************************************************************************
 *
 * Module: Common - Macros
 *
 * File Name: Common_Macros.h
 *
 * Description: Commonly used Macros
 *
 * Author: Boules Ruphael
 *
 *******************************************************************************/

#ifndef COMMON_MACROS_H
#define COMMON_MACROS_H

#include "platform_types.h"

/****************************************************************************************************************/
/*********************************************** BIT AS INPUT ***************************************************/
/****************************************************************************************************************/

/* Bit Macro to set one bit only */
#define SET_BIT(REG, BITNUM)				((REG) |= ( 1<<(BITNUM) ))
#define CLEAR_BIT(REG, BITNUM)				((REG) &= ~(1<<(BITNUM)) )
#define TOGGLE_BIT(REG, BITNUM) 			((REG) ^= ( 1<<(BITNUM) ))

//Condition
#define BIT_IS_SET(REG, BITNUM)				((REG) & ( 1<<(BITNUM) ))
#define BIT_IS_CLEAR(REG, BITNUM)			(!((REG) & ( 1<<(BITNUM) )))

//Get
#define GET_BIT(REG, BITNUM)				( ( (REG) & (1<<(BITNUM)) ) >> (BITNUM)  )


/* Create Masks */
#define CREATE_32BIT_MASK(value, BITNUM, shiftBy)				((value)<<((BITNUM)*(shiftBy)))


/* orring mutible bits by using the pin number*//*bits will create the mask*/
#define CLEAR_MULTIBLE_BITS(reg, onesMask, BITNUM, shiftBy)		((reg) &=  ~( CREATE_32BIT_MASK(onesMask, BITNUM, shiftBy) ) )
#define SETBYOR_MULTIBLE_BITS(reg, value, BITNUM, shiftBy)			((reg) |=   ( CREATE_32BIT_MASK(value, BITNUM, shiftBy) ) )
#define CLEARANDSET_MULTIBLE_BITS(reg, BITNUM, numOfBits, value)	((reg) = (((reg) & (~CREATE_32BIT_MASK(((1<<numOfBits)-1), BITNUM, numOfBits))) | (CREATE_32BIT_MASK(value, BITNUM, numOfBits))) )

enum shiftBy{
	SHIFTBY_ONE = 1, SHIFTBY_TWO = 2, SHIFTBY_THREE = 3, SHIFTBY_FOUR = 4
};

enum onesMask{
	TWO_ONES_MASK = 0b11, THREE_ONES_MASK = 0b111, FOUR_ONES_MASK = 0b1111
};






/****************************************************************************************************************/
/*********************************************** MASKS AS INPUT ***************************************************/
/****************************************************************************************************************/

/* Macro that takes Mask as input*/

/* clear bits BY MASKS of the wanted bits to clear and set*/
#define REG_CLEARANDSET_BYMASKS(REG, CLEARMASK, SETMASK)  ((REG) = (((REG) & (~(CLEARMASK))) | (SETMASK)))
#define CLEAR_AND_SET_BYMASKS(REG, CLEARMASK, SETMASK)  ((REG) = (((REG) & (~(CLEARMASK))) | (SETMASK)))


#define SET_BYMASK(REG, SETMASK)							((REG) |= (SETMASK));
#define CLEAR_BYMASK(REG, CLEARMASK)						((REG) &= ~(CLEARMASK));
#define TOGGLE_BYMASK(REG, TOGGLEMASK) 						((REG) ^= (TOGGLEMASK))

#define IS_BIT_SET(REG, BITMASK)         ((REG) & (BITMASK))
#define IS_BIT_CLR(REG, BITMASK)         (((REG) & (BITMASK)) == 0U)

/********************************end***/







/***************** asmbley lines**********/

/* Enable IRQ Interrupts ... This Macro enables IRQ interrupts by clearing the I-bit in the PRIMASK. */
#define Enable_Interrupts()    __asm("CPSIE I")

/* Disable IRQ Interrupts ... This Macro disables IRQ interrupts by setting the I-bit in the PRIMASK. */
#define Disable_Interrupts()   __asm("CPSID I")

/* Enable Exceptions ... This Macro enables Exceptions by clearing the F-bit in the FAULTMASK */
#define Enable_Exceptions()    __asm("CPSIE F")

/* Disable Exceptions ... This Macro disables Exceptions by setting the F-bit in the FAULTMASK */
#define Disable_Exceptions()   __asm("CPSID F") 

/* Go to low power mode while waiting for the next interrupt */
#define Wait_For_Interrupt()   __asm("WFI")



/********************************************************************************************************************************************/
/********************************************************************************************************************************************/
/********************************************************************************************************************************************/
/********************************************************************************************************************************************/
/********************************************************************************************************************************************/
/********************************************************************************************************************************************/
/********************************************************************************************************************************************/
/********************************************************************************************************************************************/
/********************************************************************************************************************************************/
/********************************************************************************************************************************************/

#ifndef   __STATIC_FORCEINLINE
  #define __STATIC_FORCEINLINE                   __attribute__((always_inline)) static inline
#endif
#ifndef   __ASM
  #define __ASM                                  __asm
#endif


/**
  \brief   LDR Exclusive (32 bit)
  \details Executes a exclusive LDR instruction for 32 bit values.
  \param [in]    ptr  Pointer to data
  \return        value of type uint32_t at (*ptr)
 */

__STATIC_FORCEINLINE uint32_t __LDREXW(volatile uint32_t *addr)
{
    uint32_t result;

   __ASM volatile ("ldrex %0, %1" : "=r" (result) : "Q" (*addr) );
   return(result);
}

/**
  \brief   STR Exclusive (32 bit)
  \details Executes a exclusive STR instruction for 32 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
  \return          0  Function succeeded
  \return          1  Function failed
 */
__STATIC_FORCEINLINE uint32_t __STREXW(uint32_t value, volatile uint32_t *addr)
{
   uint32_t result;

   __ASM volatile ("strex %0, %2, %1" : "=&r" (result), "=Q" (*addr) : "r" (value) );
   return(result);
}






/* Atomic 32-bit register access macro to clear one or several bits */
#define ATOMIC_CLEAR_BIT(REG, BITMASK)                           \
  do {                                                       \
    uint32_t val;                                            \
    do {                                                     \
      val = __LDREXW((__IO uint32_t *)&(REG)) & ~(BITMASK);      \
    } while ((__STREXW(val,(__IO uint32_t *)&(REG))) != 0U); \
  } while(0)


#define ATOMIC_SET_BIT(REG, BIT)                             \
  do {                                                       \
    uint32_t val;                                            \
    do {                                                     \
      val = __LDREXW((__IO uint32_t *)&(REG)) | (BIT);       \
    } while ((__STREXW(val,(__IO uint32_t *)&(REG))) != 0U); \
  } while(0)




/* Atomic 32-bit register access macro to clear and set one or several bits */
#define ATOMIC_MODIFY_REG(REG, CLEARMSK, SETMASK)                          \
  do {                                                                     \
    uint32_t val;                                                          \
    do {                                                                   \
      val = (__LDREXW((__IO uint32_t *)&(REG)) & ~(CLEARMSK)) | (SETMASK); \
    } while ((__STREXW(val,(__IO uint32_t *)&(REG))) != 0U);               \
  } while(0)

#endif /* COMMON_MACROS_H */
