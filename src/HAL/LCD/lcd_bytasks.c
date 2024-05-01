


#include "delay.h" /* For the delay functions */
#include "common_macros.h" /* To use the macros like SET_BIT */
#include "std_types.h"
#include "lcd_bytasks.h"
#include "port.h"
#include "dio.h"
#include <stdlib.h> // for itoa function

/* not basic */
ProcessState_t LCD_init_TASK(void)
{	
	ProcessState_t processState = IN_PROCESS;

	static uint8 functionProcess;
	switch (functionProcess){
		case 0:
			/* Set the control bit (RS-RW-E) as OUTPUT */
			/* Configure the direction for RS, RW and E pins as output pins */
			DIO_setupPinDirection(LCD_RS_PORT_ID,LCD_RS_PIN_ID,PIN_OUTPUT);
			DIO_setupPinDirection(LCD_RW_PORT_ID,LCD_RW_PIN_ID,PIN_OUTPUT);
			DIO_setupPinDirection(LCD_E_PORT_ID,LCD_E_PIN_ID,PIN_OUTPUT);

			/* Configure the data flow Pins(4 or 8) direction as OUTPUT */
			DIO_setupChannelDirection_offset(LCD_DATA_PORT_ID, LCD_DATA_BITS_MODE, LCD_FIRST_DATA_PIN_ID, PIN_OUTPUT);

			/* Return Cursor to LCD first line */
			if (LCD_sendCommand_TASK(LCD_GO_TO_HOME) == MINI_PROCESS_FINISHED){
				
				/* go to next process */
				functionProcess++;				
			}else {
				/*same functionProcess*/
			}
			
			processState = IN_PROCESS;
			break;

		case 1:
		#if (LCD_DATA_BITS_MODE == 4)
			/* use 2-line lcd + 4-bit Data Mode + 5*7 dot display Mode */
			if (LCD_sendCommand_TASK(LCD_TWO_LINES_FOUR_BITS_MODE) == MINI_PROCESS_FINISHED){

				/* go to next process */
				functionProcess++;				
			}else {
				/*same functionProcess*/
			}
			processState = IN_PROCESS;
		#elif (LCD_DATA_BITS_MODE == 8)
			/* use 2-line lcd + 8-bit Data Mode + 5*7 dot display Mode */
			if (LCD_sendCommand_TASK(LCD_TWO_LINES_EIGHT_BITS_MODE) == MINI_PROCESS_FINISHED){
				/* go to next process */
				functionProcess++;				
			}else {
				/*same functionProcess*/
			}
			processState = IN_PROCESS;
		#endif
			break;

		case 2:
			/* cursor off */
			if (LCD_sendCommand_TASK(LCD_CURSOR_OFF) == MINI_PROCESS_FINISHED){
				
				/* go to next process */
				functionProcess++;				
			}else {
				/*same functionProcess*/
			}
			
			processState = IN_PROCESS;
			break;

		case 3:
			/* clear LCD at the beginning */
			if (LCD_sendCommand_TASK(LCD_CLEAR_COMMAND) == MINI_PROCESS_FINISHED){
				
				/* Last function finished */
				functionProcess = 0;
				processState = ALL_PROCESSES_FINISHED;			
			}else {
				/*same functionProcess*/
				processState = IN_PROCESS;
			}
			
			
			break;

		default:
			/* Reset */
			functionProcess = 0;
			/* ERROR */
			while(1);
	}
	return processState;
}

/* send command to the LCD
 * Basic function: doesn't include mini processes functions
*/
ProcessState_t LCD_sendCommand_TASK(uint8 command)
{	
	ProcessState_t processState = IN_PROCESS;

	static uint8 functionProcess;
	switch (functionProcess){
		case 0:
			DIO_clearPin(LCD_RS_PORT_ID,LCD_RS_PIN_ID); /* Instruction Mode RS=0 */
			DIO_clearPin(LCD_RW_PORT_ID,LCD_RW_PIN_ID); /* write data to LCD so RW=0 */
			// delay_ms(1); /* delay for processing Tas = 50ns */
			processState = IN_PROCESS;
			functionProcess++;
			break;

		case 1:
			DIO_setPin(LCD_E_PORT_ID,LCD_E_PIN_ID); /* Enable LCD E=1 */
			// delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */
			processState = IN_PROCESS;
			functionProcess++;
			break;

		case 2:
		#if (LCD_DATA_BITS_MODE == 4)
			/* out the most signficant 4 bits of the required command to the data bus first D4 --> D7 */
			DIO_writeChannel_offset(LCD_DATA_PORT_ID, 4, LCD_FIRST_DATA_PIN_ID, ((command & 0xF0) >> 4) );
			// delay_ms(1); /* delay for processing Tdsw = 100ns */
			processState = IN_PROCESS;
			functionProcess++;
		#elif (LCD_DATA_BITS_MODE == 8)
			DIO_writeChannel_offset(LCD_DATA_PORT_ID, 8, LCD_FIRST_DATA_PIN_ID, command);
			// delay_ms(1); /* delay for processing Tdsw = 100ns */
			processState = IN_PROCESS;
			functionProcess++;
		#endif
			break;

		case 3:
		#if (LCD_DATA_BITS_MODE == 4)
			DIO_clearPin(LCD_E_PORT_ID,LCD_E_PIN_ID); /* Disable LCD E=0 */
			// delay_ms(1); /* delay for processing Th = 13ns */
			processState = IN_PROCESS;
			functionProcess++;
		#elif (LCD_DATA_BITS_MODE == 8)
			DIO_clearPin(LCD_E_PORT_ID,LCD_E_PIN_ID); /* Disable LCD E=0 */
			// delay_ms(1); /* delay for processing Th = 13ns */

			/* END OF FUNCTION for 8BITS MODE */
			processState = ALL_PROCESSES_FINISHED;
			functionProcess = 0;
		#endif
			break;

		case 4:
		#if (LCD_DATA_BITS_MODE == 4)
			DIO_setPin(LCD_E_PORT_ID,LCD_E_PIN_ID); /* Enable LCD E=1 */
			// delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */
			processState = IN_PROCESS;
			functionProcess++;

		#elif (LCD_DATA_BITS_MODE == 8)
		#endif
			break;

		case 5:
		#if (LCD_DATA_BITS_MODE == 4)
			/* out the least 4 bits of the required command to the data bus D4 --> D7 */
			DIO_writeChannel_offset(LCD_DATA_PORT_ID, 4, LCD_FIRST_DATA_PIN_ID, ((command & 0x0F)) );
			// delay_ms(1); /* delay for processing Tdsw = 100ns */
			processState = IN_PROCESS;
			functionProcess++;

		#elif (LCD_DATA_BITS_MODE == 8)
		#endif
			break;

		case 6:
		#if (LCD_DATA_BITS_MODE == 4)
			DIO_clearPin(LCD_E_PORT_ID,LCD_E_PIN_ID); /* Disable LCD E=0 */
			// delay_ms(1); /* delay for processing Th = 13ns */

			/* END OF FUNCTION for 8BITS MODE */
			processState = ALL_PROCESSES_FINISHED;
			functionProcess = 0;
		#elif (LCD_DATA_BITS_MODE == 8)
		#endif
			break;

		default:
			/* Reset */
			functionProcess = 0;
			/* ERROR */
			while(1);
	}

	return processState;
}

/* basic */
ProcessState_t LCD_displayCharacter_TASK(uint8 data)
{	
	ProcessState_t processState = IN_PROCESS;
	static uint8 functionProcess;
	
	switch (functionProcess){
		case 0:
			DIO_setPin(LCD_RS_PORT_ID,LCD_RS_PIN_ID); /* Data Mode RS=1 */
			DIO_clearPin(LCD_RW_PORT_ID,LCD_RW_PIN_ID); /* write data to LCD so RW=0 */
			// delay_ms(1); /* delay for processing Tas = 50ns */
			processState = IN_PROCESS;
			functionProcess++;	
			break;

		case 1:	
			DIO_setPin(LCD_E_PORT_ID,LCD_E_PIN_ID); /* Enable LCD E=1 */
			// delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */
			processState = IN_PROCESS;
			functionProcess++;
			break;
		
		case 2:
		#if (LCD_DATA_BITS_MODE == 4)
			/* out the last 4 bits of the required data to the data bus D4 --> D7 */
			DIO_writeChannel_offset(LCD_DATA_PORT_ID, LCD_DATA_BITS_MODE, LCD_FIRST_DATA_PIN_ID, ((data & 0xF0) >> 4) );
			// delay_ms(1); /* delay for processing Tdsw = 100ns */
			processState = IN_PROCESS;
			functionProcess++;
		#elif (LCD_DATA_BITS_MODE == 8)
			DIO_writeChannel_offset(LCD_DATA_PORT_ID, LCD_DATA_BITS_MODE, LCD_FIRST_DATA_PIN_ID, data); /* out the required data to the data bus D0 --> D7 */
			// delay_ms(1); /* delay for processing Tdsw = 100ns */
			processState = IN_PROCESS;
			functionProcess++;
		#endif
			break;

		case 3:
		#if (LCD_DATA_BITS_MODE == 4)
			DIO_clearPin(LCD_E_PORT_ID,LCD_E_PIN_ID); /* Disable LCD E=0 */
			// delay_ms(1); /* delay for processing Th = 13ns */
			processState = IN_PROCESS;
			functionProcess++;
		#elif (LCD_DATA_BITS_MODE == 8)
			DIO_clearPin(LCD_E_PORT_ID,LCD_E_PIN_ID); /* Disable LCD E=0 */
			// delay_ms(1); /* delay for processing Th = 13ns */

			/* END OF FUNCTION for 8BITS MODE */
			processState = ALL_PROCESSES_FINISHED;
			functionProcess = 0;
		#endif
			break;

		case 4:
		#if (LCD_DATA_BITS_MODE == 4)
			DIO_setPin(LCD_E_PORT_ID,LCD_E_PIN_ID); /* Enable LCD E=1 */
			// delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */
			processState = IN_PROCESS;
			functionProcess++;
		#endif
			break;
		
		case 5:
		#if (LCD_DATA_BITS_MODE == 4)
			/* out the first 4 bits of the required data to the data bus D0 --> D3 */
			DIO_writeChannel_offset(LCD_DATA_PORT_ID, LCD_DATA_BITS_MODE, LCD_FIRST_DATA_PIN_ID, (data & 0x0F));
			// delay_ms(1); /* delay for processing Tdsw = 100ns */
			processState = IN_PROCESS;
			functionProcess++;
		#endif
			break;

		case 6:
		#if (LCD_DATA_BITS_MODE == 4)
			DIO_clearPin(LCD_E_PORT_ID,LCD_E_PIN_ID); /* Disable LCD E=0 */
			// delay_ms(1); /* delay for processing Th = 13ns */

			/* END OF FUNCTION for 8BITS MODE */
			processState = ALL_PROCESSES_FINISHED;
			functionProcess = 0;
		#endif
			break;

		default:
			/* Reset */
			functionProcess = 0;
			/* ERROR */
			while(1);
	}


// #elif (LCD_DATA_BITS_MODE == 8)
// #endif

	return processState;
}






/* not basic */
ProcessState_t LCD_displayString_TASK(const char *Str)
{	
	ProcessState_t processState = IN_PROCESS;
	static uint8 i;				//static so it has its previous value
	if(Str[i] != '\0')			//instead of while beacuse you only do 1 process at a time (to delay) 
	{
		if (LCD_displayCharacter_TASK(Str[i]) == MINI_PROCESS_FINISHED){
			i++;

		}else{ /* if the mini process function in work */
			/* stay at the same process */
		}
		
	}else{

		/* Reset fucntion */
		i = 0;
		processState = ALL_PROCESSES_FINISHED;
	}

	return processState;
	/***************** Another Method ***********************
	while((*Str) != '\0')
	{
		LCD_displayCharacter(*Str);
		Str++;
	}		
	*********************************************************/
}

/* not basic */
ProcessState_t LCD_moveCursor_TASK(uint8 row,uint8 col)
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
	return LCD_sendCommand_TASK(lcd_memory_address | LCD_SET_CURSOR_LOCATION);
}

/* not basic */
ProcessState_t LCD_displayStringRowColumn_TASK(uint8 row,uint8 col,const char *Str)
{	
	ProcessState_t processState = IN_PROCESS;
	static uint8 functionProcess;

	switch (functionProcess){
		case 0:
			/* go to to the required LCD position */
			if (LCD_moveCursor_TASK(row,col) == MINI_PROCESS_FINISHED){
				
				/* go to next process */
				functionProcess++;				
			}else {
				/*same functionProcess*/
			}
			
			processState = IN_PROCESS;
			break;

		case 1:
			/* display the string */ 
			if (LCD_displayString_TASK(Str) == MINI_PROCESS_FINISHED){
				
				/* Last function finished */
				functionProcess = 0;
				processState = ALL_PROCESSES_FINISHED;			
			}else {
				/*same functionProcess*/
				processState = IN_PROCESS;
			}
	}

	return processState;
}

/* not basic */
ProcessState_t LCD_intgerToString_TASK(int data)
{
   char buff[16]; /* String to hold the ascii result */
   itoa(data,buff,10); /* Use itoa C function to convert the data to its corresponding ASCII value, 10 for decimal */
   return LCD_displayString_TASK(buff); /* Display the string */
}

/* not basic */
ProcessState_t LCD_clearScreen_TASK(void)
{
	return LCD_sendCommand_TASK(LCD_CLEAR_COMMAND); /* Send clear display command */
}


