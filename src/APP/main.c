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
#include "std_types.h"
#include "delay.h"
#include "port.h"
#include "usart.h"
#include "i2c.h"
// #include "systick.h"
// #include "sched.h"
#include "dio.h"
#include "lcd.h"


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
/* USER CODE END PF */

int main(void)
{

/* Initialize all configured peripherals */
	Port_Init(Port_pinConfigurationSet);
	USART_Init(&usart1Manager);
	// I2C_Init(1, &I2cConfigurationSet);



// LCD
	DIO_setPin(LCD_E_PORT_ID, LCD_E_PIN_ID);
	LCD_init(); /* Initialize the LCD */
	LCD_displayString("Welcome To LCD");
	LCD_displayStringRowColumn(1, 0, "4 Bits Data Mode");

	delay_ms(200);
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Hi, Boules");
	LCD_displayStringRowColumn(1, 0, "Hoppa FADY");
///

	// uint8 data;

	uint8 usart_recieve_buffer[5] = {'0', '0', '0', '0', '0'};
	delay_ms(1000);
	USART_startRecieve_IT(&usart1Manager, usart_recieve_buffer, 5);
	delay_ms(1000);
	USART_startTransmit_IT(&usart1Manager, usart_recieve_buffer, 5);


	// while1
	while (1){

	}


	return 0;
}


/* Templates */
//USART Polling
	/**
	 *   data = USART_recieveByte_polling(&usart1Manager); /* Receive Byte from Terminal1
	 *   USART_sendByte_polling(&usart1Manager, data);
	*/

//I2C transmit
	/**  
	 * 	uint8 i2cdata[6] = {'B', 'o', 'u', 'l', 'e', 's'};
	 * 	uint8 i2cdata2[4] = {'F', 'A', 'D', 'Y'};
	 * 
	 * 	I2C_Master_Transmit(1, 0x35, &i2cdata, 6);
	 * 	delay_ms(7000);
	 * 	I2C_Master_Transmit(1, 0x35, &i2cdata2, 4);
	 * 	delay_ms(7000);
	 */

//STK
	/** 	
	 * STK_stop();
	 * STK_setTime_ms(7000);
	 * STK_start();
	 * while ( STK_isExpire() == 0);
	 * 
	 */
