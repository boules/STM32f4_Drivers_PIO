/******************************************************************************
 *                       			Includes			                      *
 ******************************************************************************/
#include "i2c.h"
// #include "usart_cfg.h"
#include "std_types.h"


//I2C
I2C_InitTypeDef I2cConfigurationSet = {
	.ClockSpeed = 100000,
	.OwnAddress1 = (0x01),
	.AddressingMode = I2C_ADDRESSINGMODE_7BIT
};