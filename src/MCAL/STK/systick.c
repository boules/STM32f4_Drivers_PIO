#include "systick_regs.h"
#include "platform_types.h"
#include "common_macros.h"
#include "systick.h"

#ifndef F_CPU
#define F_CPU 16000000(UL)
#endif

#define MAX_COUN 16776216(UL)

/*<! count_time= 62.5ns  if 16Mhz>
 *   count_time= 0.5u       if prescaler/8 is enabled 
 */
// #define COUNT_TIME (1/F_CPU) (UL)

/** TICKTIME is cpnfigurable it is the i
 *  Systick count register is 24 bit 
 * 	then Max ticktime = 16'776'216 X COUNT_TIME
 * 
*/

/**
 * max input 8388 ms
 * you need to handle errors
*/
void STK_setTime_ms(uint16 num_of_milliseconds){
	//default SYSTICK_Time (if prescaler/8)  = 0.5u   which will make the range equals    0 >   8 seconds(~ 8388 ms) 

	// recheck that the prescaler /8 is enabled  when ==0 prescale/8 is enabled
	SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk;

	/* if max input which is 8388   the maxload == 16'776'216  which is less than 0x00ffffff the max value of the register */
	uint32 loadValue = ((num_of_milliseconds*2000)-1) & 0x00ffFFff;

	SysTick->LOAD =  (uint32) loadValue;
}

void STK_enableInterupts(){
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

void STK_disableInterupts(){
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

void STK_clearCountFlag(){
	SysTick->CTRL &= ~SysTick_CTRL_COUNTFLAG_Msk;
}

void STK_start(){
	// clear the systick counter
	SysTick->VAL =0;

	//enable the Systick
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}


void STK_stop(){
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}


uint8 STK_isExpire(){

	if ( ((SysTick->CTRL)& SysTick_CTRL_COUNTFLAG_Msk) ){
		return 1;

	}  else {
		return 0;
	}

	return 0;

} 






STK_CBFptr_t g_callBackFunction = NULL_PTR;


void STK_setCallBack(STK_CBFptr_t stkHandlerfunction){
	g_callBackFunction = stkHandlerfunction;
}

void SysTick_Handler(void){
	if(g_callBackFunction != NULL_PTR){
		g_callBackFunction();
	}

}





#if defined (__Vendor_SysTickConfig) && (__Vendor_SysTickConfig == 0U)

/**
  \brief   System Tick Configuration
  \details Initializes the System Timer and its interrupt, and starts the System Tick Timer.
           Counter is in free running mode to generate periodic interrupts.
  \param [in]  ticks  Number of ticks between two interrupts.
  \return          0  Function succeeded.
  \return          1  Function failed.
  \note    When the variable <b>__Vendor_SysTickConfig</b> is set to 1, then the
           function <b>SysTick_Config</b> is not included. In this case, the file <b><i>device</i>.h</b>
           must contain a vendor-specific implementation of this function.
 */
__STATIC_INLINE uint32_t SysTick_Config(uint32_t ticks)
{
  if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk)
  {
    return (1UL);                                                   /* Reload value impossible */
  }

  SysTick->LOAD  = (uint32_t)(ticks - 1UL);                         /* set reload register */
  NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
  SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk;                         /* Enable SysTick IRQ and SysTick Timer */
  return (0UL);                                                     /* Function successful */
}

#endif