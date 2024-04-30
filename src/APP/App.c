/* Includes ------------------------------------------------------------------*/
#include "std_types.h"
#include "delay.h"
#include "port.h"
// #include "usart.h"
// #include "i2c.h"
// #include "systick.h"
// #include "sched.h"
#include "dio.h"
#include "lcd.h"
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
	LCD_init(); /* Initialize the LCD */
	LCD_displayString("Welcome To LCD");
	LCD_displayStringRowColumn(1, 0, "8 Bits Data Mode");

	delay_ms(100);
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Hi, Boules");
	LCD_displayStringRowColumn(1, 0, "Hoppa FADY");
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
