/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "port.h"
#include "std_types.h"
#include "usart.h"
#include "i2c.h"
#include "systick.h"
#include "sched.h"
#include "dio.h"
#include "lcd.h"
#include "delay.h"

/* Private defines ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Extern variables ---------------------------------------------------------*/
/* USER CODE BEGIN EV */
extern Port_ConfigPinStruct Port_pinConfigurationSet[configuredPins];
extern USART_InitStruct UsartConfigurationSet;
extern I2C_InitTypeDef I2cConfigurationSet;
/* USER CODE END EV */

/* Private functions ------------------------------------------------------------*/
/* USER CODE BEGIN PF */
void green_led()
{
	DIO_togglePin(PORTA, PIN4);
}

void yellow_led()
{
	DIO_togglePin(PORTA, PIN5);
}

void red_led()
{
	DIO_togglePin(PORTA, PIN6);
}
/* USER CODE END PF */

int main(void)
{

	/* Initialize all configured peripherals */
	Port_Init(Port_pinConfigurationSet);
	USART_Init(&usart1Manager);
	// I2C_Init(1, &I2cConfigurationSet);

	// STK_stop();
	// STK_setTime_ms(7000);

	// uint8 data;
	// uint8 i2cdata[6] = {'B', 'o', 'u', 'l', 'e', 's'};
	// uint8 i2cdata2[4] = {'F', 'A', 'D', 'Y'};

	// I2C_Master_Transmit(1, 0x35, &i2cdata, 6);
	// STK_delay_polling();

	// I2C_Master_Transmit(1, 0x35, &i2cdata2, 4);
	// STK_delay_polling();

	// //SCHED
	// 	STK_stop();
	// 	STK_enableInterupts();
	// 	sched_init(&runnablesArray);
	// 	sched_start();

	// LCD
	DIO_setPin(LCD_E_PORT_ID, LCD_E_PIN_ID);
	LCD_init(); /* Initialize the LCD */
	LCD_displayString("Welcome To LCD");
	LCD_displayStringRowColumn(1, 0, "4 Bits Data Mode");

	delay_ms(200);
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Hi, Boules");
	LCD_displayStringRowColumn(1, 0, "Hoppa FADY");

	uint8 usart_recieve_buffer[5] = {'0', '0', '0', '0', '0'};


	delay_ms(2000);
	USART_startRecieve_IT(&usart1Manager, usart_recieve_buffer, 5);
	delay_ms(5000);

	USART_startTransmit_IT(&usart1Manager, usart_recieve_buffer, 5);


	// while1
	while (1)
	{
		// USART_startRecieve_IT(&usart1Manager, usart_recieve_buffer, 1);


	}

	// USART
	//  data = USART_recieveByte_polling(&usart1Manager); /* Receive Byte from Terminal1 */
	//  USART_sendByte_polling(&usart1Manager, data);

	return 0;
}

// /**
//   * @brief  This function is executed in case of error occurrence.
//   * @retval None
//   */
// void Error_Handler(void)
// {
//   /* USER CODE BEGIN Error_Handler_Debug */
//   /* User can add his own implementation to report the HAL error return state */
//   __disable_irq();
//   while (1)
//   {
//   }
//   /* USER CODE END Error_Handler_Debug */
// }
