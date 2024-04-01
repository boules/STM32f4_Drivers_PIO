#include "platform_types.h"
#include "scb_regs.h"
#include "nvic_regs.h"
#define __NVIC_PRIO_BITS          4U       /*!< STM32F4XX uses 4 Bits for the Priority Levels */




typedef enum InterruptRequestNumber
{
	/******  Cortex-M4 Processor Exceptions Numbers ****************************************************************/
	NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                                          */
	MemoryManagement_IRQn       = -12,    /*!< 4 Cortex-M4 Memory Management Interrupt                           */
	BusFault_IRQn               = -11,    /*!< 5 Cortex-M4 Bus Fault Interrupt                                   */
	UsageFault_IRQn             = -10,    /*!< 6 Cortex-M4 Usage Fault Interrupt                                 */
	SVCall_IRQn                 = -5,     /*!< 11 Cortex-M4 SV Call Interrupt                                    */
	DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M4 Debug Monitor Interrupt                              */
	PendSV_IRQn                 = -2,     /*!< 14 Cortex-M4 Pend SV Interrupt                                    */
	SysTick_IRQn                = -1,     /*!< 15 Cortex-M4 System Tick Interrupt                                */
	/******  STM32 specific Interrupt Numbers **********************************************************************/
	WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt                                         */
	PVD_IRQn                    = 1,      /*!< PVD through EXTI Line detection Interrupt                         */
	TAMP_STAMP_IRQn             = 2,      /*!< Tamper and TimeStamp interrupts through the EXTI line             */
	RTC_WKUP_IRQn               = 3,      /*!< RTC Wakeup interrupt through the EXTI line                        */
	FLASH_IRQn                  = 4,      /*!< FLASH global Interrupt                                            */
	RCC_IRQn                    = 5,      /*!< RCC global Interrupt                                              */
	EXTI0_IRQn                  = 6,      /*!< EXTI Line0 Interrupt                                              */
	EXTI1_IRQn                  = 7,      /*!< EXTI Line1 Interrupt                                              */
	EXTI2_IRQn                  = 8,      /*!< EXTI Line2 Interrupt                                              */
	EXTI3_IRQn                  = 9,      /*!< EXTI Line3 Interrupt                                              */
	EXTI4_IRQn                  = 10,     /*!< EXTI Line4 Interrupt                                              */
	DMA1_Stream0_IRQn           = 11,     /*!< DMA1 Stream 0 global Interrupt                                    */
	DMA1_Stream1_IRQn           = 12,     /*!< DMA1 Stream 1 global Interrupt                                    */
	DMA1_Stream2_IRQn           = 13,     /*!< DMA1 Stream 2 global Interrupt                                    */
	DMA1_Stream3_IRQn           = 14,     /*!< DMA1 Stream 3 global Interrupt                                    */
	DMA1_Stream4_IRQn           = 15,     /*!< DMA1 Stream 4 global Interrupt                                    */
	DMA1_Stream5_IRQn           = 16,     /*!< DMA1 Stream 5 global Interrupt                                    */
	DMA1_Stream6_IRQn           = 17,     /*!< DMA1 Stream 6 global Interrupt                                    */
	ADC_IRQn                    = 18,     /*!< ADC1, ADC2 and ADC3 global Interrupts                             */
	EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                                     */
	TIM1_BRK_TIM9_IRQn          = 24,     /*!< TIM1 Break interrupt and TIM9 global interrupt                    */
	TIM1_UP_TIM10_IRQn          = 25,     /*!< TIM1 Update Interrupt and TIM10 global interrupt                  */
	TIM1_TRG_COM_TIM11_IRQn     = 26,     /*!< TIM1 Trigger and Commutation Interrupt and TIM11 global interrupt */
	TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                                    */
	TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                                             */
	TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                                             */
	TIM4_IRQn                   = 30,     /*!< TIM4 global Interrupt                                             */
	I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                              */
	I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                              */
	I2C2_EV_IRQn                = 33,     /*!< I2C2 Event Interrupt                                              */
	I2C2_ER_IRQn                = 34,     /*!< I2C2 Error Interrupt                                              */
	SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                             */
	SPI2_IRQn                   = 36,     /*!< SPI2 global Interrupt                                             */
	USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                                           */
	USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                                           */
	EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                                   */
	RTC_Alarm_IRQn              = 41,     /*!< RTC Alarm (A and B) through EXTI Line Interrupt                   */
	OTG_FS_WKUP_IRQn            = 42,     /*!< USB OTG FS Wakeup through EXTI line interrupt                     */
	DMA1_Stream7_IRQn           = 47,     /*!< DMA1 Stream7 Interrupt                                            */
	SDIO_IRQn                   = 49,     /*!< SDIO global Interrupt                                             */
	TIM5_IRQn                   = 50,     /*!< TIM5 global Interrupt                                             */
	SPI3_IRQn                   = 51,     /*!< SPI3 global Interrupt                                             */
	DMA2_Stream0_IRQn           = 56,     /*!< DMA2 Stream 0 global Interrupt                                    */
	DMA2_Stream1_IRQn           = 57,     /*!< DMA2 Stream 1 global Interrupt                                    */
	DMA2_Stream2_IRQn           = 58,     /*!< DMA2 Stream 2 global Interrupt                                    */
	DMA2_Stream3_IRQn           = 59,     /*!< DMA2 Stream 3 global Interrupt                                    */
	DMA2_Stream4_IRQn           = 60,     /*!< DMA2 Stream 4 global Interrupt                                    */
	OTG_FS_IRQn                 = 67,     /*!< USB OTG FS global Interrupt                                       */
	DMA2_Stream5_IRQn           = 68,     /*!< DMA2 Stream 5 global interrupt                                    */
	DMA2_Stream6_IRQn           = 69,     /*!< DMA2 Stream 6 global interrupt                                    */
	DMA2_Stream7_IRQn           = 70,     /*!< DMA2 Stream 7 global interrupt                                    */
	USART6_IRQn                 = 71,     /*!< USART6 global interrupt                                           */
	I2C3_EV_IRQn                = 72,     /*!< I2C3 event interrupt                                              */
	I2C3_ER_IRQn                = 73,     /*!< I2C3 error interrupt                                              */
	FPU_IRQn                    = 81,     /*!< FPU global interrupt                                              */
	SPI4_IRQn                   = 84      /*!< SPI4 global Interrupt                                              */
} IRQn_Type;









































// /**
//   \brief   Set Priority Grouping
//   \details Sets the priority grouping field using the required unlock sequence.
//            The parameter PriorityGroup is assigned to the field SCB->AIRCR [10:8] PRIGROUP field.
//            Only values from 0..7 are used.
//            In case of a conflict between priority grouping and available
//            priority bits (__NVIC_PRIO_BITS), the smallest possible priority group is set.
//   \param [in]      PriorityGroup  Priority grouping field.
//  */
// static inline void __NVIC_SetPriorityGrouping(uint32 PriorityGroup)
// {
//   uint32 reg_value;
//   uint32 PriorityGroupTmp = (PriorityGroup & (uint32)0x07UL);             /* only values 0..7 are used          */

//   reg_value  =  SCB->AIRCR;                                                   /* read old register configuration    */
//   reg_value &= ~((uint32)(SCB_AIRCR_VECTKEY_Msk | SCB_AIRCR_PRIGROUP_Msk)); /* clear bits to change               */
//   reg_value  =  (reg_value                                   |
//                 ((uint32)0x5FAUL << SCB_AIRCR_VECTKEY_Pos) |
//                 (PriorityGroupTmp << SCB_AIRCR_PRIGROUP_Pos)  );              /* Insert write key and priority group */
//   SCB->AIRCR =  reg_value;
// }


// /**
//   \brief   Get Priority Grouping
//   \details Reads the priority grouping field from the NVIC Interrupt Controller.
//   \return                Priority grouping field (SCB->AIRCR [10:8] PRIGROUP field).
//  */
// static inline uint32 __NVIC_GetPriorityGrouping(void)
// {
//   return ((uint32)((SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) >> SCB_AIRCR_PRIGROUP_Pos));
// }










// /**
//   \brief   Encode Priority
//   \details Encodes the priority for an interrupt with the given priority group,
//            preemptive priority value, and subpriority value.
//            In case of a conflict between priority grouping and available
//            priority bits (__NVIC_PRIO_BITS), the smallest possible priority group is set.
//   \param [in]     PriorityGroup  Used priority group.
//   \param [in]   PreemptPriority  Preemptive priority value (starting from 0).
//   \param [in]       SubPriority  Subpriority value (starting from 0).
//   \return                        Encoded priority. Value can be used in the function \ref NVIC_SetPriority().
//  */
// static inline uint32 NVIC_EncodePriority (uint32 PriorityGroup, uint32 PreemptPriority, uint32 SubPriority)
// {
//   uint32 PriorityGroupTmp = (PriorityGroup & (uint32)0x07UL);   /* only values 0..7 are used          */
//   uint32 PreemptPriorityBits;
//   uint32 SubPriorityBits;

//   PreemptPriorityBits = ((7UL - PriorityGroupTmp) > (uint32)(__NVIC_PRIO_BITS)) ? (uint32)(__NVIC_PRIO_BITS) : (uint32)(7UL - PriorityGroupTmp);
//   SubPriorityBits     = ((PriorityGroupTmp + (uint32)(__NVIC_PRIO_BITS)) < (uint32)7UL) ? (uint32)0UL : (uint32)((PriorityGroupTmp - 7UL) + (uint32)(__NVIC_PRIO_BITS));

//   return (
//            ((PreemptPriority & (uint32)((1UL << (PreemptPriorityBits)) - 1UL)) << SubPriorityBits) |
//            ((SubPriority     & (uint32)((1UL << (SubPriorityBits    )) - 1UL)))
//          );
// }


// /**
//   \brief   Decode Priority
//   \details Decodes an interrupt priority value with a given priority group to
//            preemptive priority value and subpriority value.
//            In case of a conflict between priority grouping and available
//            priority bits (__NVIC_PRIO_BITS) the smallest possible priority group is set.
//   \param [in]         Priority   Priority value, which can be retrieved with the function \ref NVIC_GetPriority().
//   \param [in]     PriorityGroup  Used priority group.
//   \param [out] pPreemptPriority  Preemptive priority value (starting from 0).
//   \param [out]     pSubPriority  Subpriority value (starting from 0).
//  */
// static inline void NVIC_DecodePriority (uint32 Priority, uint32 PriorityGroup, uint32* const pPreemptPriority, uint32* const pSubPriority)
// {
//   uint32 PriorityGroupTmp = (PriorityGroup & (uint32)0x07UL);   /* only values 0..7 are used          */
//   uint32 PreemptPriorityBits;
//   uint32 SubPriorityBits;

//   PreemptPriorityBits = ((7UL - PriorityGroupTmp) > (uint32)(__NVIC_PRIO_BITS)) ? (uint32)(__NVIC_PRIO_BITS) : (uint32)(7UL - PriorityGroupTmp);
//   SubPriorityBits     = ((PriorityGroupTmp + (uint32)(__NVIC_PRIO_BITS)) < (uint32)7UL) ? (uint32)0UL : (uint32)((PriorityGroupTmp - 7UL) + (uint32)(__NVIC_PRIO_BITS));

//   *pPreemptPriority = (Priority >> SubPriorityBits) & (uint32)((1UL << (PreemptPriorityBits)) - 1UL);
//   *pSubPriority     = (Priority                   ) & (uint32)((1UL << (SubPriorityBits    )) - 1UL);
// }


// /**
//   \brief   Set Interrupt Vector
//   \details Sets an interrupt vector in SRAM based interrupt vector table.
//            The interrupt number can be positive to specify a device specific interrupt,
//            or negative to specify a processor exception.
//            VTOR must been relocated to SRAM before.
//   \param [in]   IRQn      Interrupt number
//   \param [in]   vector    Address of interrupt handler function
//  */
// static inline void __NVIC_SetVector(IRQn_Type IRQn, uint32 vector)
// {
//   uint32 *vectors = (uint32 *)SCB->VTOR;
//   vectors[(int32_t)IRQn + NVIC_USER_IRQ_OFFSET] = vector;
//   /* ARM Application Note 321 states that the M4 does not require the architectural barrier */
// }


// /**
//   \brief   Get Interrupt Vector
//   \details Reads an interrupt vector from interrupt vector table.
//            The interrupt number can be positive to specify a device specific interrupt,
//            or negative to specify a processor exception.
//   \param [in]   IRQn      Interrupt number.
//   \return                 Address of interrupt handler function
//  */
// static inline uint32 __NVIC_GetVector(IRQn_Type IRQn)
// {
//   uint32 *vectors = (uint32 *)SCB->VTOR;
//   return vectors[(int32_t)IRQn + NVIC_USER_IRQ_OFFSET];
// }









/*******************************************Enable IRQ****************************************/
static inline void __NVIC_EnableIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
	NVIC->ISER[((IRQn) /32)] = (uint32)(1UL << ((IRQn) % 32));
	
  }
}

static inline void __NVIC_DisableIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
	NVIC->ICER[((IRQn) /32)] = (uint32)(1UL << ((IRQn) % 32));
	
  }
}
// static inline uint32 __NVIC_GetEnableIRQ(IRQn_Type IRQn)
// {
//   if ((int32_t)(IRQn) >= 0)
//   {
//     return((uint32)(((NVIC->ISER[(((uint32)IRQn) /32)] & (1UL << ((IRQn) %32))) != 0UL) ? 1UL : 0UL));
//   }
//   else
//   {
//     return(0U);
//   }
// }



/***********************************Pending IRQ************************************************/
static inline void __NVIC_SetPendingIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    NVIC->ISPR[((IRQn) /32)] = (uint32)(1UL << ((IRQn) % 32));
  }
}


static inline void __NVIC_ClearPendingIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    NVIC->ICPR[((IRQn) /32)] = (uint32)(1UL << ((IRQn) % 32));
  }
}


static inline uint32 __NVIC_GetPendingIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    return((uint32)(((NVIC->ISPR[((IRQn) /32)] & (1UL << ((IRQn) %32))) != 0UL) ? 1UL : 0UL));
  }
  else
  {
    return(0U);
  }
}



/*************************************GET Active IRQ*******************************************/
static inline uint32 __NVIC_GetActive(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    return((uint32)(((NVIC->IABR[((IRQn) /32)] & (1UL << ((IRQn) %32))) != 0UL) ? 1UL : 0UL));
  }
  else
  {
    return(0U);
  }
}



///////////////////////////////

/**
  \brief   Set Interrupt Priority
  \details Sets the priority of a device specific interrupt or a processor exception.
           The interrupt number can be positive to specify a device specific interrupt,
           or negative to specify a processor exception.
  \param [in]      IRQn  Interrupt number.
  \param [in]  priority  Priority to set.
  \note    The priority cannot be set for every processor exception.
 */
static inline void __NVIC_SetPriority(IRQn_Type IRQn, uint32 priority)
{
  if ((int32_t)(IRQn) >= 0)
  {
    NVIC->IP[((uint32)IRQn)]               = (uint8_t)((priority << (8U - __NVIC_PRIO_BITS)) & (uint32)0xFFUL);
  }
  else
  {
    SCB->SHP[(((uint32)IRQn) & 0xFUL)-4UL] = (uint8_t)((priority << (8U - __NVIC_PRIO_BITS)) & (uint32)0xFFUL);
  }
}


/**
  \brief   Get Interrupt Priority
  \details Reads the priority of a device specific interrupt or a processor exception.
           The interrupt number can be positive to specify a device specific interrupt,
           or negative to specify a processor exception.
  \param [in]   IRQn  Interrupt number.
  \return             Interrupt Priority.
                      Value is aligned automatically to the implemented priority bits of the microcontroller.
 */
static inline uint32 __NVIC_GetPriority(IRQn_Type IRQn)
{

  if ((int32_t)(IRQn) >= 0)
  {
    return(((uint32)NVIC->IP[((uint32)IRQn)]               >> (8U - __NVIC_PRIO_BITS)));
  }
  else
  {
    return(((uint32)SCB->SHP[(((uint32)IRQn) & 0xFUL)-4UL] >> (8U - __NVIC_PRIO_BITS)));
  }
}
