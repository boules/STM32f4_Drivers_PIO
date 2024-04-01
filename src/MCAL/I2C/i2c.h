
#include "platform_types.h"
#include <stdint.h>


typedef enum
{
  I2C_ADDRESSINGMODE_7BIT         = 0x00004000U,	/*!< bit15  in a 16bit register             */

} I2C_AddressingMode_t;

typedef struct
{
  uint32 ClockSpeed;       /*!< Specifies the clock frequency.
                                  This parameter must be set to a value lower than 400kHz */

//   uint32_t DutyCycle;        /*!< Specifies the I2C fast mode duty cycle.
//                                   This parameter can be a value of @ref I2C_duty_cycle_in_fast_mode */

  uint32 OwnAddress1;      /*!< Specifies the first device own address.
                                  This parameter can be a 7-bit or 10-bit address. */

  I2C_AddressingMode_t AddressingMode;   /*!< Specifies if 7-bit or 10-bit addressing mode is selected.
  												This parameter can be a value of @ref I2C_addressing_mode */

//   uint32_t DualAddressMode;  /*!< Specifies if dual addressing mode is selected.
//                                   This parameter can be a value of @ref I2C_dual_addressing_mode */

//   uint32_t OwnAddress2;      /*!< Specifies the second device own address if dual addressing mode is selected
//                                   This parameter can be a 7-bit address. */

//   uint32_t GeneralCallMode;  /*!< Specifies if general call mode is selected.
//                                   This parameter can be a value of @ref I2C_general_call_addressing_mode */

//   uint32_t NoStretchMode;    /*!< Specifies if nostretch mode is selected.
//                                   This parameter can be a value of @ref I2C_nostretch_mode */

} I2C_InitTypeDef;



typedef enum
{
  HAL_I2C_MODE_NONE               = 0x00U,   /*!< No I2C communication on going             */
  HAL_I2C_MODE_MASTER             = 0x10U,   /*!< I2C communication is in Master Mode       */
  HAL_I2C_MODE_SLAVE              = 0x20U,   /*!< I2C communication is in Slave Mode        */
  HAL_I2C_MODE_MEM                = 0x40U    /*!< I2C communication is in Memory Mode       */

} HAL_I2C_ModeTypeDef;







/***0*/
// typedef struct
// {
//   I2C_TypeDef                *Instance;      /*!< I2C registers base address               */

//   I2C_InitTypeDef            Init;           /*!< I2C communication parameters             */

//   uint8                    *pBuffPtr;      /*!< Pointer to I2C transfer buffer           */

//   uint16                   XferSize;       /*!< I2C transfer size                        */

//   __IO uint16              XferCount;      /*!< I2C transfer counter                     */

//   __IO uint32              XferOptions;    /*!< I2C transfer options                     */

//   __IO uint32              PreviousState;  /*!< I2C communication Previous state and mode
//                                                   context for internal usage               */

//   HAL_LockTypeDef            Lock;           /*!< I2C locking object                       */

//   __IO HAL_I2C_StateTypeDef  State;          /*!< I2C communication state                  */

//   __IO HAL_I2C_ModeTypeDef   Mode;           /*!< I2C communication mode                   */

//   __IO uint32_t              ErrorCode;      /*!< I2C Error code                           */

//   __IO uint32_t              Devaddress;     /*!< I2C Target device address                */

//   __IO uint32_t              Memaddress;     /*!< I2C Target memory address                */

//   __IO uint32_t              MemaddSize;     /*!< I2C Target memory address  size          */

//   __IO uint32_t              EventCount;     /*!< I2C Event counter                        */

// } I2C_HandleTypeDef;
/*****/









#define I2C_ADDRESSINGMODE_7BIT         0x00004000U      //bit15  in a 16bit register






void I2C_Init(uint8 I2Cmodule, I2C_InitTypeDef* configPtr);
void I2C_Master_Transmit(uint8 I2Cmodule, uint16 DevAddress, uint8 *pData, uint8 length);
void I2C_Slave_Receive(uint8 I2Cmodule, uint8 *pData, uint8 length);

// void I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16 DevAddress, uint8 *pData, uint16 Size, uint32 Timeout);
// void I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16 DevAddress, uint8 *pData, uint16 Size, uint32 Timeout);
// void I2C_Slave_Transmit(I2C_HandleTypeDef *hi2c, uint8 *pData, uint16 Size, uint32 Timeout);
// void I2C_Slave_Receive(I2C_HandleTypeDef *hi2c, uint8 *pData, uint16 Size, uint32 Timeout);
