 /******************************************************************************
 *
 * Module: LCD
 *
 * File Name: lcd.h
 *
 * Description: Header file for the LCD driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#ifndef LCD_H_
#define LCD_H_

#include "std_types.h"
#include "lcd_cfg.h"

/******************************************************************************
*                                Definitions                                  *
*******************************************************************************/
/* LCD Commands */
#define LCD_CLEAR_COMMAND              0x01				/*clear screen and reset cursor*/
#define LCD_GO_TO_HOME                 0x02				/*reset cursor  but does't Clear the Screen(DDram)*/
#define LCD_TWO_LINES_EIGHT_BITS_MODE  0x38
#define LCD_TWO_LINES_FOUR_BITS_MODE   0x28
#define LCD_CURSOR_OFF                 0x0C
#define LCD_CURSOR_ON                  0x0E
#define LCD_SET_CURSOR_LOCATION        0x80				/*sets cursor location at given address sent with it by orring*/ /*cusor at the begininig of the 1st raw*/    /*0xc0   begining of 2nd Raw*/


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Initialize the LCD:
 * 1. Setup the LCD pins directions by use the GPIO driver.
 * 2. Setup the LCD Data Mode 4-bits or 8-bits.
 */
void LCD_init(void);

/*
 * Description :
 * Send the required command to the screen
 */
void LCD_sendCommand(uint8 command);

/*
 * Description :
 * Display the required character on the screen
 */
void LCD_displayCharacter(uint8 data);

/*
 * Description :
 * Display the required string on the screen
 */
void LCD_displayString(const char *Str);

/*
 * Description :
 * Move the cursor to a specified row and column index on the screen
 */
void LCD_moveCursor(uint8 row,uint8 col);

/*
 * Description :
 * Display the required string in a specified row and column index on the screen
 */
void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *Str);

/*
 * Description :
 * Display the required decimal value on the screen
 */
void LCD_intgerToString(int data);

/*
 * Description :
 * Send the clear screen command
 */
void LCD_clearScreen(void);

#endif /* LCD_H_ */
