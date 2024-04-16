#ifndef KEYPAD_CFG_H
#define KEYPAD_CFG_H


#include "std_types.h"


/******************************************************************************
*                                Definitions                                  *
*******************************************************************************/

/* Keypad configurations for number of rows and columns */
#define KEYPAD_NUM_COLS                  4
#define KEYPAD_NUM_ROWS                  4

/* Keypad Port Configurations */
#define KEYPAD_PORT_ID                   PORTB

#define KEYPAD_FIRST_ROW_PIN_ID           PIN0
#define KEYPAD_FIRST_COLUMN_PIN_ID        PIN4


/* Keypad button logic configurations */
#define KEYPAD_BUTTON_PRESSED            LOGIC_HIGH				/* (if logic high)  Remeber to ADD (Pull_Down) to the ROW pins only(the read from pins)*//*if low add pull up*/
#define KEYPAD_BUTTON_RELEASED           LOGIC_LOW



#endif /* KEYPAD_CFG_H */