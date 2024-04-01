


#ifndef PLATFORM_TYPES_H
#include "platform_types.h"
#endif




/*RCC_AHB1ENR*/
#define RCC_GPIOA		0
#define RCC_GPIOB		1
#define RCC_GPIOC		2
#define RCC_GPIOD		3
#define RCC_GPIOE		4
#define RCC_GPIOF		5
#define RCC_GPIOG		6
#define RCC_GPIOH		7
#define RCC_CRC			8
#define RCC_DMA1		9
#define RCC_DMA2		10

/*!< APB1 peripherals APB1ENR*/
#define RCC_TIM2		11
#define RCC_TIM3		12
#define RCC_TIM4		13
#define RCC_TIM5		14
#define RCC_WWDG		15
#define RCC_SPI2		16
#define RCC_SPI3		17
#define RCC_USART2		18
#define RCC_I2C1		19
#define RCC_I2C2		20
#define RCC_I2C3		21
#define RCC_PWR			22

/*!< APB2 peripherals APB2ENR*/
#define RCC_TIM1		23
#define RCC_USART1		24
#define RCC_USART6		25
#define RCC_ADC1		26
#define RCC_SDIO		27
#define RCC_SPI1		28
#define RCC_SPI4		29
#define RCC_SYSCFG		30
#define RCC_TIM9		31
#define RCC_TIM10		32
#define RCC_TIM11		33













void RCC_Enable(uint8 moduleName);

