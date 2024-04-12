#ifndef LCD_CFG_H
#define LCD_CFG_H







/*************************************************************************************/
/***************************** LCD MODE Configurations *******************************/
/*************************************************************************************/
/*------------------------------------- LCD mode ---------------------------------------*/
/* LCD Data bits mode configuration, its value should be 4 or 8*/

#define LCD_DATA_BITS_MODE 4

#if((LCD_DATA_BITS_MODE != 4) && (LCD_DATA_BITS_MODE != 8))
#error "Number of Data bits should be equal to 4 or 8"
#endif



/**************************************************************************************/
/****************************** LCD PINS CONFIGURATIONs *******************************/
/**************************************************************************************/
/*------------------------------ control pins config -----------------------------------*/
/* LCD HW Ports and Pins Ids */

#define LCD_RS_PORT_ID                 PORTB
#define LCD_RS_PIN_ID                  PIN13

#define LCD_RW_PORT_ID                 PORTB
#define LCD_RW_PIN_ID                  PIN14

#define LCD_E_PORT_ID                  PORTB
#define LCD_E_PIN_ID                   PIN15


/*--------------------------------- DATA pins config -----------------------------------*/

#define LCD_DATA_PORT_ID				PORTC
#define LCD_FIRST_DATA_PIN_ID			PIN0				/*PORT offset*/
/**************************************************************************************/













#endif //LCD_CFG_H