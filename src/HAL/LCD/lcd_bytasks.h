#ifndef LCD_BYTASKS_H
#define LCD_BYTASKS_H

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



ProcessState_t LCD_init_TASK(void);
ProcessState_t LCD_sendCommand_TASK(uint8 command);
ProcessState_t LCD_displayCharacter_TASK(uint8 data);

ProcessState_t LCD_displayString_TASK(const char *Str);
ProcessState_t LCD_moveCursor_TASK(uint8 row,uint8 col);
ProcessState_t LCD_displayStringRowColumn_TASK(uint8 row,uint8 col,const char *Str);
ProcessState_t LCD_intgerToString_TASK(int data);
ProcessState_t LCD_clearScreen_TASK(void);

#endif