 /******************************************************************************
 *
 * Module: LCD
 *
 * File Name: lcd.c
 *
 * Description: Source file for the LCD driver
 *
 * Author: Boules Ruphael
 *
 *******************************************************************************/

#include "delay.h" /* For the delay functions */
#include "common_macros.h" /* To use the macros like SET_BIT */
#include "lcd.h"
#include "port.h"
#include "dio.h"
#include <stdlib.h> // for itoa function

//you might use stdlib.h for itoa function
//if 4 bits mode   you must send the most significant bits first
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Initialize the LCD:
 * 1. Setup the LCD pins directions by use the GPIO driver.
 * 2. Setup the LCD Data Mode 4-bits or 8-bits.
 */
void LCD_init(void)
{	
	/* Set the control bit (RS-RW-E) as OUTPUT */
	/* Configure the direction for RS, RW and E pins as output pins */
	DIO_setupPinDirection(LCD_RS_PORT_ID,LCD_RS_PIN_ID,PIN_OUTPUT);
	DIO_setupPinDirection(LCD_RW_PORT_ID,LCD_RW_PIN_ID,PIN_OUTPUT);
	DIO_setupPinDirection(LCD_E_PORT_ID,LCD_E_PIN_ID,PIN_OUTPUT);

	/* Configure the data flow Pins(4 or 8) direction as OUTPUT */
	DIO_setupChannelDirection_offset(LCD_DATA_PORT_ID, LCD_DATA_BITS_MODE, LCD_FIRST_DATA_PIN_ID, PIN_OUTPUT);

	LCD_sendCommand(LCD_GO_TO_HOME);
	
#if (LCD_DATA_BITS_MODE == 4)
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE); /* use 2-line lcd + 4-bit Data Mode + 5*7 dot display Mode */
#elif (LCD_DATA_BITS_MODE == 8)
	LCD_sendCommand(LCD_TWO_LINES_EIGHT_BITS_MODE); /* use 2-line lcd + 8-bit Data Mode + 5*7 dot display Mode */
#endif


	LCD_sendCommand(LCD_CURSOR_OFF); /* cursor off */
	LCD_sendCommand(LCD_CLEAR_COMMAND); /* clear LCD at the beginning */
}

/*
 * Description :
 * Send the required command to the screen
 */
void LCD_sendCommand(uint8 command)
{
	DIO_clearPin(LCD_RS_PORT_ID,LCD_RS_PIN_ID); /* Instruction Mode RS=0 */
	DIO_clearPin(LCD_RW_PORT_ID,LCD_RW_PIN_ID); /* write data to LCD so RW=0 */
	delay_ms(1); /* delay for processing Tas = 50ns */
	DIO_setPin(LCD_E_PORT_ID,LCD_E_PIN_ID); /* Enable LCD E=1 */
	delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */


#if (LCD_DATA_BITS_MODE == 4)
	/* out the most signficant 4 bits of the required command to the data bus first D4 --> D7 */
	DIO_writeChannel_offset(LCD_DATA_PORT_ID, 4, LCD_FIRST_DATA_PIN_ID, ((command & 0xF0) >> 4) );

	delay_ms(1); /* delay for processing Tdsw = 100ns */
	DIO_clearPin(LCD_E_PORT_ID,LCD_E_PIN_ID); /* Disable LCD E=0 */
	delay_ms(1); /* delay for processing Th = 13ns */
	DIO_setPin(LCD_E_PORT_ID,LCD_E_PIN_ID); /* Enable LCD E=1 */
	delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */

	/* out the least 4 bits of the required command to the data bus D4 --> D7 */
	DIO_writeChannel_offset(LCD_DATA_PORT_ID, 4, LCD_FIRST_DATA_PIN_ID, ((command & 0x0F)) );

	delay_ms(1); /* delay for processing Tdsw = 100ns */
	DIO_clearPin(LCD_E_PORT_ID,LCD_E_PIN_ID); /* Disable LCD E=0 */
	delay_ms(1); /* delay for processing Th = 13ns */

#elif (LCD_DATA_BITS_MODE == 8)
	DIO_writeChannel_offset(LCD_DATA_PORT_ID, 8, LCD_FIRST_DATA_PIN_ID, command);
	delay_ms(1); /* delay for processing Tdsw = 100ns */
	DIO_clearPin(LCD_E_PORT_ID,LCD_E_PIN_ID); /* Disable LCD E=0 */
	delay_ms(1); /* delay for processing Th = 13ns */
#endif
}

/*
 * Description :
 * the main lcd option
 * Sends the display characters data (the lcd itself increase the address no need to handle it)
 * Display the required character on the screen
 */
void LCD_displayCharacter(uint8 data)
{
	DIO_setPin(LCD_RS_PORT_ID,LCD_RS_PIN_ID); /* Data Mode RS=1 */
	DIO_clearPin(LCD_RW_PORT_ID,LCD_RW_PIN_ID); /* write data to LCD so RW=0 */
	delay_ms(1); /* delay for processing Tas = 50ns */

	DIO_setPin(LCD_E_PORT_ID,LCD_E_PIN_ID); /* Enable LCD E=1 */
	delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */


#if (LCD_DATA_BITS_MODE == 4)
	/* out the last 4 bits of the required data to the data bus D4 --> D7 */
	DIO_writeChannel_offset(LCD_DATA_PORT_ID, LCD_DATA_BITS_MODE, LCD_FIRST_DATA_PIN_ID, ((data & 0xF0) >> 4) );
	delay_ms(1); /* delay for processing Tdsw = 100ns */

	DIO_clearPin(LCD_E_PORT_ID,LCD_E_PIN_ID); /* Disable LCD E=0 */
	delay_ms(1); /* delay for processing Th = 13ns */

	DIO_setPin(LCD_E_PORT_ID,LCD_E_PIN_ID); /* Enable LCD E=1 */
	delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */

	/* out the first 4 bits of the required data to the data bus D0 --> D3 */
	DIO_writeChannel_offset(LCD_DATA_PORT_ID, LCD_DATA_BITS_MODE, LCD_FIRST_DATA_PIN_ID, (data & 0x0F));
	delay_ms(1); /* delay for processing Tdsw = 100ns */

	DIO_clearPin(LCD_E_PORT_ID,LCD_E_PIN_ID); /* Disable LCD E=0 */
	delay_ms(1); /* delay for processing Th = 13ns */

#elif (LCD_DATA_BITS_MODE == 8)
	DIO_writeChannel_offset(LCD_DATA_PORT_ID, LCD_DATA_BITS_MODE, LCD_FIRST_DATA_PIN_ID, data); /* out the required data to the data bus D0 --> D7 */
	delay_ms(1); /* delay for processing Tdsw = 100ns */

	DIO_clearPin(LCD_E_PORT_ID,LCD_E_PIN_ID); /* Disable LCD E=0 */
	delay_ms(1); /* delay for processing Th = 13ns */
#endif


}

























































/*
 * Description :
 * Display the required string on the screen
 */
void LCD_displayString(const char *Str)
{
	uint8 i = 0;
	while(Str[i] != '\0')
	{
		LCD_displayCharacter(Str[i]);
		i++;
	}
	/***************** Another Method ***********************
	while((*Str) != '\0')
	{
		LCD_displayCharacter(*Str);
		Str++;
	}		
	*********************************************************/
}

/*
 * Description :
 * Move the cursor to a specified row and column index on the screen
 */
void LCD_moveCursor(uint8 row,uint8 col)
{
	uint8 lcd_memory_address;
	
	/* Calculate the required address in the LCD DDRAM */
	switch(row)
	{
		case 0:
			lcd_memory_address=col;
				break;
		case 1:
			lcd_memory_address=col+0x40;
				break;
		case 2:
			lcd_memory_address=col+0x10;
				break;
		case 3:
			lcd_memory_address=col+0x50;
				break;
		default:
			//if row is wrong input move curser to first row
			lcd_memory_address=col;
			break;
	}					
	/* Move the LCD cursor to this specific address */
	LCD_sendCommand(lcd_memory_address | LCD_SET_CURSOR_LOCATION);
}

/*
 * Description :
 * Display the required string in a specified row and column index on the screen
 */
void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *Str)
{
	LCD_moveCursor(row,col); /* go to to the required LCD position */
	LCD_displayString(Str); /* display the string */
}

/*
 * Description :
 * Display the required decimal value on the screen
 */
void LCD_intgerToString(int data)
{
   char buff[16]; /* String to hold the ascii result */
   itoa(data,buff,10); /* Use itoa C function to convert the data to its corresponding ASCII value, 10 for decimal */
   LCD_displayString(buff); /* Display the string */
}

/*
 * Description :
 * Send the clear screen command
 */
void LCD_clearScreen(void)
{
	LCD_sendCommand(LCD_CLEAR_COMMAND); /* Send clear display command */
}
