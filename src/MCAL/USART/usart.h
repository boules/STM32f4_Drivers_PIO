#ifndef USART_H
#define USART_H

#include "platform_types.h"
#include "usart_regs.h"









enum {
	USART1, USART2, USART6
};


#define USART_WORDLENGTH_8B                  0x00000000U
#define USART_WORDLENGTH_9B                  ((uint32_t)USART_CR1_M)					// 0x00001000


#define USART_STOPBITS_1                     0x00000000U
#define USART_STOPBITS_2                     ((uint32_t)USART_CR2_STOP_1)				// 0x2000

#define USART_PARITY_NONE                    0x00000000U
#define USART_PARITY_EVEN                    ((uint32_t)USART_CR1_PCE)					// 0x00000400
#define USART_PARITY_ODD                     ((uint32_t)(USART_CR1_PCE | USART_CR1_PS))	// 0x00000600

#define USART_MODE_RX                        ((uint32_t)USART_CR1_RE)					// 0x00000004
#define USART_MODE_TX                        ((uint32_t)USART_CR1_TE)					// 0x00000008
#define USART_MODE_TX_RX                     ((uint32_t)(USART_CR1_TE | USART_CR1_RE))	// 0x0000000C





#define USART_ERROR_NONE              0x00000000U   		/*!< No error            */
#define USART_ERROR_PE                USART_SR_PE_Msk	/*0x00000001U*/   /*!< Parity error        */
#define USART_ERROR_FE                USART_SR_FE_Msk	/*0x00000002U*/   /*!< Frame error         */
#define USART_ERROR_NE                USART_SR_NE_Msk	/*0x00000004U*/   /*!< Noise error         */
#define USART_ERROR_ORE               USART_SR_ORE_Msk	/*0x00000008U*/   /*!< Overrun error       */
#define USART_ERROR_DMA               0x00000010U		/*!< DMA transfer error  */





typedef struct
{
  uint32 BaudRate;                  /*!< This member configures the UART communication baud rate.
                                           The baud rate is computed using the following formula:
                                           - IntegerDivider = ((PCLKx) / (8 * (OVR8+1) * (huart->Init.BaudRate)))
                                           - FractionalDivider = ((IntegerDivider - ((uint32_t) IntegerDivider)) * 8 * (OVR8+1)) + 0.5
                                           Where OVR8 is the "oversampling by 8 mode" configuration bit in the CR1 register. */

  uint32 WordLength;                /*!< Specifies the number of data bits transmitted or received in a frame.
                                           This parameter can be a value of @ref UART_Word_Length */

  uint32 StopBits;                  /*!< Specifies the number of stop bits transmitted.
                                           This parameter can be a value of @ref UART_Stop_Bits */

  uint32 Parity;                    /*!< Specifies the parity mode.
                                           This parameter can be a value of @ref UART_Parity
                                           @note When parity is enabled, the computed parity is inserted
                                                 at the MSB position of the transmitted data (9th bit when
                                                 the word length is set to 9 data bits; 8th bit when the
                                                 word length is set to 8 data bits). */


  uint32 Mode;                      /*!< Specifies whether the Receive or Transmit mode is enabled or disabled.
                                           This parameter can be a value of @ref UART_Mode */

//   uint32 HwFlowCtl;                 /*!< Specifies whether the hardware flow control mode is enabled or disabled.
//                                            This parameter can be a value of @ref UART_Hardware_Flow_Control */

//   uint32 OverSampling;              /*!< Specifies whether the Over sampling 8 is enabled or disabled, to achieve higher speed (up to fPCLK/8).
//                                            This parameter can be a value of @ref UART_Over_Sampling */
} USART_InitStruct;





// typedef struct{
// 	uint32 uart_number;
// 	UART_InitTypeDef* initStruct;
// 	uint8 state;


// } UART_Handler;


void USART_Init(uint8 usart_number, USART_InitStruct *configPtr);
void USART_sendByte_polling(uint8 usart_number, const uint8 data);
uint8 USART_recieveByte_polling(uint8 usart_number);





typedef enum USART_state_t{
	READY = 0,
	BUSY  = 1,

} USART_state_t;

uint8_t bufferData[3];

typedef enum
{
  USART_STATE_RESET             = 0x00U,    /*!< Peripheral is not yet Initialized
                                                   Value is allowed for gState and RxState */
  USART_STATE_READY             = 0x20U,    /*!< Peripheral Initialized and ready for use
                                                   Value is allowed for gState and RxState */
  USART_STATE_BUSY              = 0x24U,    /*!< an internal process is ongoing
                                                   Value is allowed for gState only */
  USART_STATE_BUSY_TX           = 0x21U,    /*!< Data Transmission process is ongoing
                                                   Value is allowed for gState only */
  USART_STATE_BUSY_RX           = 0x22U,    /*!< Data Reception process is ongoing
                                                   Value is allowed for RxState only */
  USART_STATE_BUSY_TX_RX        = 0x23U,    /*!< Data Transmission and Reception process is ongoing
                                                   Not to be used for neither gState nor RxState.
                                                   Value is result of combination (Or) between gState and RxState values */
  USART_STATE_TIMEOUT           = 0xA0U,    /*!< Timeout state
                                                   Value is allowed for gState only */
  USART_STATE_ERROR             = 0xE0U     /*!< Error
                                                   Value is allowed for gState only */
} USART_STATE_t;


typedef struct USART_ManagerStruct
{
  USART_RegStruct				*moduleBase;        /*!< UART registers base address        */

  USART_InitStruct				init;             /*!< UART communication parameters      */

  const uint8_t                 *pTxBuffPtr;      /*!< Pointer to UART Tx transfer Buffer */

  uint16_t                      TxXferSize;       /*!< UART Tx Transfer size              */

  __IO uint16_t                 TxXferCount;      /*!< UART Tx Transfer Counter           */

  uint8_t                       *pRxBuffPtr;      /*!< Pointer to UART Rx transfer Buffer */

  uint16_t                      RxXferSize;       /*!< UART Rx Transfer size              */

  __IO uint16_t                 RxXferCount;      /*!< UART Rx Transfer Counter           */



  __IO USART_STATE_t			txState;           /*!< UART state information related to global Handle management
//                                                        and also related to Tx operations.
//                                                        This parameter can be a value of @ref HAL_UART_StateTypeDef */

  __IO USART_STATE_t			rxState;          /*!< UART state information related to Rx operations.
//                                                        This parameter can be a value of @ref HAL_UART_StateTypeDef */

  __IO uint32_t                 ErrorCode;        /*!< UART Error code                    */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
  void (* TxHalfCpltCallback)(struct __UART_HandleTypeDef *huart);        /*!< UART Tx Half Complete Callback        */
  void (* TxCpltCallback)(struct __UART_HandleTypeDef *huart);            /*!< UART Tx Complete Callback             */
  void (* RxHalfCpltCallback)(struct __UART_HandleTypeDef *huart);        /*!< UART Rx Half Complete Callback        */
  void (* RxCpltCallback)(struct __UART_HandleTypeDef *huart);            /*!< UART Rx Complete Callback             */
  void (* ErrorCallback)(struct __UART_HandleTypeDef *huart);             /*!< UART Error Callback                   */
  void (* AbortCpltCallback)(struct __UART_HandleTypeDef *huart);         /*!< UART Abort Complete Callback          */
  void (* AbortTransmitCpltCallback)(struct __UART_HandleTypeDef *huart); /*!< UART Abort Transmit Complete Callback */
  void (* AbortReceiveCpltCallback)(struct __UART_HandleTypeDef *huart);  /*!< UART Abort Receive Complete Callback  */
  void (* WakeupCallback)(struct __UART_HandleTypeDef *huart);            /*!< UART Wakeup Callback                  */
  void (* RxEventCallback)(struct __UART_HandleTypeDef *huart, uint16_t Pos); /*!< UART Reception Event Callback     */

  void (* MspInitCallback)(struct __UART_HandleTypeDef *huart);           /*!< UART Msp Init callback                */
  void (* MspDeInitCallback)(struct __UART_HandleTypeDef *huart);         /*!< UART Msp DeInit callback              */
#endif  /* USE_HAL_UART_REGISTER_CALLBACKS */

} USART_ManagerStruct;



#endif