// #define MAIN_2
#ifdef MAIN_2

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
// #include "i2c.h"
// #include "systick.h"
// #include "sched.h"
#include "dio.h"
#include "lcd.h"
#include "dma.h"
#include "nvic.h"
#include "keypad.h"


/* Private defines ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Global variables ---------------------------------------------------------*/
/* USER CODE BEGIN GV */
/* USER CODE END GV */

/* Extern variables ---------------------------------------------------------*/
/* USER CODE BEGIN EV */

//-----------init configuration sets------
extern Port_ConfigPinStruct Port_pinConfigurationSet[configuredPins];
extern USART_InitStruct UsartConfigurationSet;
// extern I2C_InitTypeDef I2cConfigurationSet;
//------------managers & handlers----------
extern USART_ManagerStruct usart1Manager;
extern DMA_HandleTypeDef dma2Manager_stream2_usart1_rx;
extern DMA_HandleTypeDef dma2Manager_stream7_usart1_tx;
// extern DMA_HandleTypeDef dma2Manager_stream0_memtomem;

/* USER CODE END EV */

/* Private functions ------------------------------------------------------------*/
/* USER CODE BEGIN PF */
/* USER CODE END PF */

int main(void)
{

/* Initialize all configured peripherals */
	Port_Init(&Port_pinConfigurationSet);
	// USART_Init(&usart1Manager);
	// I2C_Init(1, &I2cConfigurationSet);
	// DMA_Init(&dma2Manager_stream0_memtomem);
	// DMA_Init(&dma2Manager_stream2_usart1_rx);
	// DMA_Init(&dma2Manager_stream7_usart1_tx);



// LCD
	LCD_init(); /* Initialize the LCD */
	LCD_displayString("Welcome To LCD");
	LCD_displayStringRowColumn(1, 0, "8 Bits Data Mode");

	delay_ms(100);
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Hi, Boules");
	LCD_displayStringRowColumn(1, 0, "Hoppa FADY");
///





	while (1);
	return 0;
}























/* Templates */
//USART Polling
#define USART_POLLING_TEST
#ifndef USART_POLLING_TEST
	data = USART_recieveByte_polling(&usart1Manager); // Receive Byte from Terminal1
	USART_sendByte_polling(&usart1Manager, data);
#endif

//I2C transmit
#define I2C_TRANSMIT_TEST
#ifndef I2C_TRANSMIT_TEST
	uint8 i2cdata[6] = {'B', 'o', 'u', 'l', 'e', 's'};
	uint8 i2cdata2[4] = {'F', 'A', 'D', 'Y'};

	I2C_Master_Transmit(1, 0x35, &i2cdata, 6);
	delay_ms(7000);
	I2C_Master_Transmit(1, 0x35, &i2cdata2, 4);
	delay_ms(7000);
#endif

//STK polling
#ifdef STK_polling
	 	
	STK_stop();
	STK_setTime_ms(7000);
	STK_start();
	while ( STK_isExpire() == 0);

#endif

//DMA memtomem
#ifdef DMA_memtomem
	/**
	 */
	// volatile uint8 srcVariable = 5;
	// volatile uint8 dstVariable = 0;
	// // uint32 distinationAddress = 0x0800C000;
	// DMA_start(&dma2Manager_stream0_memtomem, (uint32)&srcVariable, (uint32)&dstVariable, 1);
	// if( DMA_PollForTransfer(&dma2Manager_stream0_memtomem, HAL_DMA_FULL_TRANSFER)    == MCAL_ERROR)
	// {	
	// 	/*error*/
	// 	while(1);
	// }
	// if (srcVariable == dstVariable){
	// 	// success
	// 	while(1){
	// 	}
	// }else {
	// 	// fail
	// 	while(1){
	// 	}
	// }
	 /* 
	*/
#endif

//UART interrupts testing 
#ifdef UART_interrupts_testing
	// uint8 data;
	uint8 usart_recieve_buffer[5] = {'0', '0', '0', '0', '0'};
	delay_ms(100);
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
#endif

#define USART_TRANSMIT_DMA_TEST
#ifndef USART_TRANSMIT_DMA_TEST
	volatile uint8 variable1[6] = {'b', 'o', 'u', 'l', 'e', 's'};
	volatile uint8 variable2[6] = {'f', 'a', 'd', 'y', 'm', 'o'};

	// USART_Receive_DMA(&usart1Manager, &variable1, 6);
	USART_Transmit_DMA(&usart1Manager, &variable2, 6);



	// while1
	while (1){
		if (variable1[0] == variable2[0]){
			// success 0
			while(1){
					if (variable1[1] == variable2[1]){
						// success 1
						while(1){
							if (variable1[2] == variable2[2]){
								// success 1
								while(1){
									if (variable1[3] == variable2[3]){
										// success 1
										while(1){
											if (variable1[4] == variable2[4]){
												// success 1
												while(1){
													if (variable1[5] == variable2[5]){
														// success 1
														while(1){

														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
			}
		}
	}
#endif

#define KEYPAD_TEST
#ifndef KEYPAD_TEST
	uint8 data = 0;
	uint8 dataPrevious= 0;
	while (1)
	{
		dataPrevious = data;
		data = KEYPAD_getPressedKey();

		if(data != dataPrevious){
			USART_sendByte_polling(&usart1Manager, data);
		}
		
	}
#endif

#endif /*MAIN_2*/
