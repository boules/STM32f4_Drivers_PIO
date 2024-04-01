#ifndef PORT_H
#define PORT_H

/******************************************************************************
 *                       			Includes			                      *
 ******************************************************************************/
#include "Port_Cfg.h"
#include <platform_types.h>


/********************************************************************************************
 *                       					Defines											*
*********************************************************************************************/
/********************  Port & Pins Defines  ********************/
/* MCU PORTS Define*/
enum PORT_NUM {
	PORTA, PORTB, PORTC, PORTD, PORTE, PORTF, PORTG, PORTH, PORTI
};

enum PIN_NUM {
	PIN0, PIN1, PIN2,  PIN3,  PIN4,  PIN5,  PIN6,  PIN7,
	PIN8, PIN9, PIN10, PIN11, PIN12, PIN13, PIN14, PIN15
};

/**********************  Functions codename Defines  ****************************
* macros for pin different Modes,
* assigned to mode of type Port_PinModeType */
/* Alternate function for 4Bits(MCU)  */
#define PORT_ALTERNATE_FUNCTION_00_MODE         (0U)                            /*!< SYS>*/
#define PORT_ALTERNATE_FUNCTION_01_MODE         (1U)                            /*!< TIM 1/2>*/
#define PORT_ALTERNATE_FUNCTION_02_MODE         (2U)                            /*!< TIM 3/4/5>*/
#define PORT_ALTERNATE_FUNCTION_03_MODE         (3U)                            /*!< TIM 8/9/10/11>*/
#define PORT_ALTERNATE_FUNCTION_04_MODE         (4U)                            /*!< I2C 1/2/3>*/
#define PORT_ALTERNATE_FUNCTION_05_MODE         (5U)                            /*!< SPI1/SPI2/ I2S2/I2S2ext>*/
#define PORT_ALTERNATE_FUNCTION_06_MODE         (6U)                            /*!< SPI3/ I2Sext/I2S3>*/
#define PORT_ALTERNATE_FUNCTION_07_MODE         (7U)                            /*!< USART1/2/3/ I2S3ext>*/
#define PORT_ALTERNATE_FUNCTION_08_MODE         (8U)                            /*!< UART4/5/ USART6>*/
#define PORT_ALTERNATE_FUNCTION_09_MODE         (9U)                            /*!< CAN1/2 TIM12/13/14>*/
#define PORT_ALTERNATE_FUNCTION_10_MODE         (10U)                           /*!< OTG_FS/OTG_HS>*/
#define PORT_ALTERNATE_FUNCTION_11_MODE         (11U)                           /*!< ETH>*/
#define PORT_ALTERNATE_FUNCTION_12_MODE         (12U)                           /*!< FSMC/SDIO /OTG_FS>*/
#define PORT_ALTERNATE_FUNCTION_13_MODE         (13U)                           /*!< DCMI>*/
#define PORT_ALTERNATE_FUNCTION_14_MODE         (14U)
#define PORT_ALTERNATE_FUNCTION_15_MODE         (15U)

/********Not in the AF configurstions *************/
#define PORT_DIO_MODE 				(16U)
#define PORT_ANALOG_MODE			(17U)


/********************  Modes configurations Defines  ********************/

typedef enum{
  GPIO_Mode_IN   = 0x00, /*!< GPIO Input Mode */
  GPIO_Mode_OUT  = 0x01, /*!< GPIO Output Mode */
  GPIO_Mode_AF   = 0x02, /*!< GPIO Alternate function Mode */
  GPIO_Mode_AN   = 0x03  /*!< GPIO Analog Mode */
}GPIOMode_TypeDef;

/* 3. PIN direction autosar */
typedef enum{
  PORT_PIN_IN,
  PORT_PIN_OUT
}Port_PinDirectionType;

/* Structure that contains all the configurations features of a single pin */
typedef struct Port_ConfigPinStruct{ 
	uint8				port;
	uint8				pin;

	uint8				mode;						/* GPIO_Mode_IN...GPIO_Mode_AN */
	uint8				mode_changeability;			/* CHANGEABLE, UNCHANGEABLE */

	uint8				direction;					/* PORT_PIN_IN, PORT_PIN_OUT */
	uint8				direction_changeability;	/* STD_ON, STD_OFF */
	uint8				GPIO_PuPd;					/* No_Pull, Pull_Up, Pull_Down */
	uint8				GPIO_OType;					/* OType_PP,  OType_OD */

	uint8				initial_value;				/* STD_HIGH, STD_LOW */
	uint8				GPIO_Speed;					/* GPIO_Speed_2MHz(GPIO_SPEED_FREQ_LOW), GPIO_Speed_25MHz, GPIO_Speed_50MHz, GPIO_Speed_100MHz */
	uint8				AlternateFunction;			/**/

}Port_ConfigPinStruct;

#define DEFAULT_FEATURE					0x00U

#define CHANGEABLE                      0x00U
#define UNCHANGEABLE                    0x01U

#define  GPIO_SPEED_FREQ_LOW         0x00000000U  /*!< IO works at 2 MHz, please refer to the product datasheet */
#define  GPIO_SPEED_FREQ_MEDIUM      0x00000001U  /*!< range 12,5 MHz to 50 MHz, please refer to the product datasheet */
#define  GPIO_SPEED_FREQ_HIGH        0x00000002U  /*!< range 25 MHz to 100 MHz, please refer to the product datasheet  */
#define  GPIO_SPEED_FREQ_VERY_HIGH   0x00000003U  /*!< range 50 MHz to 200 MHz, please refer to the product datasheet  */





/*functions*/
void Port_Init(Port_ConfigPinStruct configArr[]);
/*******************/



#endif /* PORT_H */