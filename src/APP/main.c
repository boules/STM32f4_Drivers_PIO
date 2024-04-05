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
#include "dma.h"


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

extern USART_ManagerStruct usart1Manager;
extern DMA_HandleTypeDef dma2Manager_stream2_usart1_rx;
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
	DMA_Init(dma2Manager_stream2_usart1_rx);



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

//UART interrupts testing 
	// uint8 data;
	uint8 usart_recieve_buffer[5] = {'0', '0', '0', '0', '0'};
	delay_ms(1000);
	if (USART_startReceive_IT(&usart1Manager, usart_recieve_buffer, 5) != MCAL_OK){
		while(1){}
	}
	// USART_TransmitData_IT(USART_ManagerStruct *usartxManger)
	delay_ms(1000);
	if (USART_startTransmit_IT(&usart1Manager, usart_recieve_buffer, 5) != MCAL_OK){
		while (1)
		{
			
		}
	}




	DMA_Init(dmaManager_usart1_rx);
	/* USART1 interrupt Init */
	HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);

	HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
	/* DMA2_Stream7_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);


	// while1
	while (1){

	}


	return 0;
}


/* Templates */
//USART Polling
	/**
	 *   data = USART_recieveByte_polling(&usart1Manager); // Receive Byte from Terminal1
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
