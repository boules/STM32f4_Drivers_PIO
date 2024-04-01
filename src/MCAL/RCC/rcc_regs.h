
#ifndef PORT_REGS_H
#define PORT_REGS_H

/******************************************************************************
 *                       			Includes			                      *
 ******************************************************************************/
#ifndef PLATFORM_TYPES_H
#include "platform_types.h"
#endif






/*******************************************************************************
 *                        RCC Registers Structure Mapping                      *
 ******************************************************************************/
typedef struct
{
	volatile uint32 CR;            /*!< RCC clock control register,                                  Address offset: 0x00 */
	volatile uint32 PLLCFGR;       /*!< RCC PLL configuration register,                              Address offset: 0x04 */
	volatile uint32 CFGR;          /*!< RCC clock configuration register,                            Address offset: 0x08 */
	volatile uint32 CIR;           /*!< RCC clock interrupt register,                                Address offset: 0x0C */
	volatile uint32 AHB1RSTR;      /*!< RCC AHB1 peripheral reset register,                          Address offset: 0x10 */
	volatile uint32 AHB2RSTR;      /*!< RCC AHB2 peripheral reset register,                          Address offset: 0x14 */
	volatile uint32 AHB3RSTR;      /*!< RCC AHB3 peripheral reset register,                          Address offset: 0x18 */
	uint32      RESERVED0;     /*!< Reserved, 0x1C                                                                    */
	volatile uint32 APB1RSTR;      /*!< RCC APB1 peripheral reset register,                          Address offset: 0x20 */
	volatile uint32 APB2RSTR;      /*!< RCC APB2 peripheral reset register,                          Address offset: 0x24 */
	uint32      RESERVED1[2];  /*!< Reserved, 0x28-0x2C                                                               */
	volatile uint32 AHB1ENR;       /*!< RCC AHB1 peripheral clock register,                          Address offset: 0x30 */
	volatile uint32 AHB2ENR;       /*!< RCC AHB2 peripheral clock register,                          Address offset: 0x34 */
	volatile uint32 AHB3ENR;       /*!< RCC AHB3 peripheral clock register,                          Address offset: 0x38 */
	uint32      RESERVED2;     /*!< Reserved, 0x3C                                                                    */
	volatile uint32 APB1ENR;       /*!< RCC APB1 peripheral clock enable register,                   Address offset: 0x40 */
	volatile uint32 APB2ENR;       /*!< RCC APB2 peripheral clock enable register,                   Address offset: 0x44 */
	uint32      RESERVED3[2];  /*!< Reserved, 0x48-0x4C                                                               */
	volatile uint32 AHB1LPENR;     /*!< RCC AHB1 peripheral clock enable in low power mode register, Address offset: 0x50 */
	volatile uint32 AHB2LPENR;     /*!< RCC AHB2 peripheral clock enable in low power mode register, Address offset: 0x54 */
	volatile uint32 AHB3LPENR;     /*!< RCC AHB3 peripheral clock enable in low power mode register, Address offset: 0x58 */
	uint32      RESERVED4;     /*!< Reserved, 0x5C                                                                    */
	volatile uint32 APB1LPENR;     /*!< RCC APB1 peripheral clock enable in low power mode register, Address offset: 0x60 */
	volatile uint32 APB2LPENR;     /*!< RCC APB2 peripheral clock enable in low power mode register, Address offset: 0x64 */
	uint32      RESERVED5[2];  /*!< Reserved, 0x68-0x6C                                                               */
	volatile uint32 BDCR;          /*!< RCC Backup domain control register,                          Address offset: 0x70 */
	volatile uint32 CSR;           /*!< RCC clock control & status register,                         Address offset: 0x74 */
	uint32      RESERVED6[2];  /*!< Reserved, 0x78-0x7C                                                               */
	volatile uint32 SSCGR;         /*!< RCC spread spectrum clock generation register,               Address offset: 0x80 */
	volatile uint32 PLLI2SCFGR;    /*!< RCC PLLI2S configuration register,                           Address offset: 0x84 */
	uint32      RESERVED7[1];  /*!< Reserved, 0x88                                                                    */
	volatile uint32 DCKCFGR;       /*!< RCC Dedicated Clocks configuration register,                 Address offset: 0x8C */

} RCC_TypeDef;



/********************  RCC Base Address  ***************/
#define PERIPH_BASE				(0x40000000UL)								/*!< Peripheral base address in the alias region*/
#define AHB1PERIPH_BASE			(PERIPH_BASE + 0x00020000)
#define RCC_BASE				(AHB1PERIPH_BASE + 0x3800)
#define RCC_regPtr				((RCC_TypeDef *) RCC_BASE)









/*******************************************************************************
 *                       RCC Registers configuration masks	                   *
 ******************************************************************************/



/********************  Bit definition for RCC_AHB1ENR register Mask ***************/
#define  RCC_AHB1ENR_GPIOAEN				((uint32)0x00000001)
#define  RCC_AHB1ENR_GPIOBEN				((uint32)0x00000002)
#define  RCC_AHB1ENR_GPIOCEN				((uint32)0x00000004)
#define  RCC_AHB1ENR_GPIODEN				((uint32)0x00000008)
#define  RCC_AHB1ENR_GPIOEEN				((uint32)0x00000010)
#define  RCC_AHB1ENR_GPIOFEN				((uint32)0x00000020)
#define  RCC_AHB1ENR_GPIOGEN				((uint32)0x00000040)
#define  RCC_AHB1ENR_GPIOHEN				((uint32)0x00000080)
#define  RCC_AHB1ENR_GPIOIEN				((uint32)0x00000100)
#define  RCC_AHB1ENR_CRCEN					((uint32)0x00001000)
#define  RCC_AHB1ENR_BKPSRAMEN				((uint32)0x00040000)
#define  RCC_AHB1ENR_CCMDATARAMEN			((uint32)0x00100000)
#define  RCC_AHB1ENR_DMA1EN					((uint32)0x00200000)
#define  RCC_AHB1ENR_DMA2EN					((uint32)0x00400000)
#define  RCC_AHB1ENR_ETHMACEN				((uint32)0x02000000)
#define  RCC_AHB1ENR_ETHMACTXEN				((uint32)0x04000000)
#define  RCC_AHB1ENR_ETHMACRXEN				((uint32)0x08000000)
#define  RCC_AHB1ENR_ETHMACPTPEN			((uint32)0x10000000)
#define  RCC_AHB1ENR_OTGHSEN				((uint32)0x20000000)
#define  RCC_AHB1ENR_OTGHSULPIEN			((uint32)0x40000000)


/*************************************************************************************************************************************/
// need to be sorted
/*RCC_APB1ENR*/
/********************  Bit definition for RCC_APB1ENR register  ***************/
#define RCC_APB1ENR_TIM2EN                 ((uint32)0x00000001)
#define RCC_APB1ENR_TIM3EN                 ((uint32)0x00000002)
#define RCC_APB1ENR_TIM4EN                 ((uint32)0x00000004)
#define RCC_APB1ENR_TIM5EN                 ((uint32)0x00000008)
#define RCC_APB1ENR_WWDGEN                 ((uint32)0x00000800)
#define RCC_APB1ENR_SPI2EN                 ((uint32)0x00004000)
#define RCC_APB1ENR_SPI3EN                 ((uint32)0x00008000)
#define RCC_APB1ENR_USART2EN               ((uint32)0x00020000)
#define RCC_APB1ENR_I2C1EN                 ((uint32)0x00200000)
#define RCC_APB1ENR_I2C2EN                 ((uint32)0x00400000)
#define RCC_APB1ENR_I2C3EN                 ((uint32)0x00800000)
#define RCC_APB1ENR_PWREN                  ((uint32)0x10000000)

/********************  Bit definition for RCC_APB2ENR register  ***************/
#define RCC_APB2ENR_TIM1EN                 ((uint32)0x00000001)
#define RCC_APB2ENR_USART1EN               ((uint32)0x00000010)
#define RCC_APB2ENR_USART6EN               ((uint32)0x00000020)
#define RCC_APB2ENR_ADC1EN                 ((uint32)0x00000100)
#define RCC_APB2ENR_SDIOEN                 ((uint32)0x00000800)
#define RCC_APB2ENR_SPI1EN                 ((uint32)0x00001000)
#define RCC_APB2ENR_SPI4EN                 ((uint32)0x00002000)
#define RCC_APB2ENR_SYSCFGEN               ((uint32)0x00004000)
#define RCC_APB2ENR_TIM9EN                 ((uint32)0x00010000)
#define RCC_APB2ENR_TIM10EN                ((uint32)0x00020000)
#define RCC_APB2ENR_TIM11EN                ((uint32)0x00040000)




/**************************************************/

#endif /* PORT_REGS_H */
