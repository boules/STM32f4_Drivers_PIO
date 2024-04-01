#include "rcc_regs.h"
#include "rcc.h"


/*
* 1. function that hadles RCC enables
*
*
**/

void RCC_Enable(uint8 moduleName){
	switch (moduleName){
		/******************************************************************************************************/
		/*RCC_AHB1ENR*/
		case RCC_GPIOA:
			RCC_regPtr->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
			break;
		case RCC_GPIOB:
			RCC_regPtr->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
			break;
		case RCC_GPIOC:
			RCC_regPtr->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
			break;
		case RCC_GPIOD:
			RCC_regPtr->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
			break;
		case RCC_GPIOE:
			RCC_regPtr->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
			break;
		case RCC_GPIOF:
			RCC_regPtr->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
			break;
		case RCC_GPIOG:
			RCC_regPtr->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
			break;
		case RCC_GPIOH:
			RCC_regPtr->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
			break;
		case RCC_CRC:
			RCC_regPtr->AHB1ENR |= RCC_AHB1ENR_CRCEN;
			break;
		case RCC_DMA1:
			RCC_regPtr->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
			break;
		case RCC_DMA2:
			RCC_regPtr->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
			break;


		/******************************/
		/*RCC_APB1ENR*/
		case RCC_TIM2:
			RCC_regPtr->APB1ENR |= RCC_APB1ENR_TIM2EN;
			break;
		case RCC_TIM3:
			RCC_regPtr->APB1ENR |= RCC_APB1ENR_TIM3EN;
			break;
		case RCC_TIM4:
			RCC_regPtr->APB1ENR |= RCC_APB1ENR_TIM4EN;
			break;
		case RCC_TIM5:
			RCC_regPtr->APB1ENR |= RCC_APB1ENR_TIM5EN;
			break;
		case RCC_WWDG:
			RCC_regPtr->APB1ENR |= RCC_APB1ENR_WWDGEN;
			break;
		case RCC_SPI2:
			RCC_regPtr->APB1ENR |= RCC_APB1ENR_SPI2EN;
			break;
		case RCC_SPI3:
			RCC_regPtr->APB1ENR |= RCC_APB1ENR_SPI3EN;
			break;
		case RCC_USART2:
			RCC_regPtr->APB1ENR |= RCC_APB1ENR_USART2EN;
			break;
		case RCC_I2C1:
			RCC_regPtr->APB1ENR |= RCC_APB1ENR_I2C1EN;
			break;
		case RCC_I2C2:
			RCC_regPtr->APB1ENR |= RCC_APB1ENR_I2C2EN;
			break;
		case RCC_I2C3:
			RCC_regPtr->APB1ENR |= RCC_APB1ENR_I2C3EN;
			break;
		case RCC_PWR:
			RCC_regPtr->APB1ENR |= RCC_APB1ENR_PWREN;
			break;


		/******************************************************************/
		/*RCC_APB2ENR*/
		case RCC_TIM1:
			RCC_regPtr->APB2ENR |= RCC_APB2ENR_TIM1EN;
			break;
		case RCC_USART1:
			RCC_regPtr->APB2ENR |= RCC_APB2ENR_USART1EN;
			break;
		case RCC_USART6:
			RCC_regPtr->APB2ENR |= RCC_APB2ENR_USART6EN;
			break;
		case RCC_ADC1:
			RCC_regPtr->APB2ENR |= RCC_APB2ENR_ADC1EN;
			break;
		case RCC_SDIO:
			RCC_regPtr->APB2ENR |= RCC_APB2ENR_SDIOEN;
			break;
		case RCC_SPI1:
			RCC_regPtr->APB2ENR |= RCC_APB2ENR_SPI1EN;
			break;
		case RCC_SPI4:
			RCC_regPtr->APB2ENR |= RCC_APB2ENR_SPI4EN;
			break;
		case RCC_SYSCFG:
			RCC_regPtr->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
			break;
		case RCC_TIM9:
			RCC_regPtr->APB2ENR |= RCC_APB2ENR_TIM9EN;
			break;
		case RCC_TIM10:
			RCC_regPtr->APB2ENR |= RCC_APB2ENR_TIM10EN;
			break;
		case RCC_TIM11:
			RCC_regPtr->APB2ENR |= RCC_APB2ENR_TIM11EN;
			break;



		default:
			/* hand*/
			break;
	}



}


