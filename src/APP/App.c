/* Includes ------------------------------------------------------------------*/
#include "std_types.h"
#include "delay.h"
#include "port.h"
// #include "usart.h"
// #include "i2c.h"
// #include "systick.h"
// #include "sched.h"
#include "dio.h"
#include "lcd_bytasks.h"
// #include "dma.h"
// #include "nvic.h"
// #include "keypad.h"
#include "led.h"

//-----------init configuration sets------
extern Port_ConfigPinStruct Port_pinConfigurationSet[configuredPins];
// extern USART_InitStruct UsartConfigurationSet;
// // extern I2C_InitTypeDef I2cConfigurationSet;
// //------------managers & handlers----------
// extern USART_ManagerStruct usart1Manager;
// extern DMA_HandleTypeDef dma2Manager_stream2_usart1_rx;
// extern DMA_HandleTypeDef dma2Manager_stream7_usart1_tx;
// // extern DMA_HandleTypeDef dma2Manager_stream0_memtomem;






/* Description: Task executes once to initialize all the Modules */
void Init_Task(void)
{
/* Initialize all configured peripherals */
	Port_Init(&Port_pinConfigurationSet);
	// USART_Init(&usart1Manager);
	// I2C_Init(1, &I2cConfigurationSet);
	// DMA_Init(&dma2Manager_stream0_memtomem);
	// DMA_Init(&dma2Manager_stream2_usart1_rx);
	// DMA_Init(&dma2Manager_stream7_usart1_tx);

//LCD
	// LCD_init(); /* Initialize the LCD */

// LCD NORMAL
	// LCD_init(); /* Initialize the LCD */
	// LCD_displayString("Welcome To LCD");
	// LCD_displayStringRowColumn(1, 0, "8 Bits Data Mode");

	// delay_ms(100);
	// LCD_clearScreen();
	// LCD_displayStringRowColumn(0, 0, "Hi, Boules");
	// LCD_displayStringRowColumn(1, 0, "Hoppa FADY");
///
}



void Green_Led_Task(void){
	LED_toggle(GreenLED);
}

void Yellow_Led_Task(void){
	LED_toggle(YellowLED);
}

void Red_Led_Task(void){
	LED_toggle(RedLED);
}

void LCD_Main_Task(void){ //ProcessState_t

	static uint8 TaskProcess;
	switch(TaskProcess){
		case 0:
			/* Initialize the LCD */
			if (LCD_init_TASK() == MINI_PROCESS_FINISHED){
				TaskProcess++;
			} else {
				/* same process */
			}
			break;
		case 1:
			if (LCD_displayString_TASK("Welcome To LCD") == MINI_PROCESS_FINISHED){
				TaskProcess++;
			} else {
				/* same process */
			}
			break;
		
		case 2:
			if (LCD_displayStringRowColumn_TASK(1, 0, "8 Bits Data Mode") == MINI_PROCESS_FINISHED){
				
				/* TASK FINISHED SUCCESSFULLY */
				TaskProcess++;

			} else {
				/* same process */
			}
			break;
		
		default:
			/* TASK FINISHED SUCCESSFULLY */
			/* do nothing */;
	}


}
