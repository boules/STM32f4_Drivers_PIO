/******************************************************************************
 *                       			Includes			                      *
 ******************************************************************************/
#include "i2c.h"
#include "i2c_regs.h"
#include "std_types.h"


//I2C
I2C_ManagerStruct i2c1Manager = {
	Instance= I2C1,
	Init = {
		.ClockSpeed = 100000,
		.OwnAddress1 = (0x01),
		.AddressingMode = I2C_ADDRESSINGMODE_7BIT
	}

}

I2C_InitTypeDef I2cConfigurationSet = {
	.ClockSpeed = 100000,
	.OwnAddress1 = (0x01),
	.AddressingMode = I2C_ADDRESSINGMODE_7BIT
};