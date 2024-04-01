/******************************************************************************
 *                       			Includes			                      *
 ******************************************************************************/
#include "usart.h"
#include "usart_regs.h"//for dev phase only
// #include "usart_cfg.h"
#include "std_types.h"

//USART
USART_InitStruct UsartConfigurationSet = {
	.BaudRate = 9600,
	.WordLength = USART_WORDLENGTH_8B,
	.StopBits = USART_STOPBITS_1,
	.Parity = USART_PARITY_NONE,
	.Mode = USART_MODE_TX_RX
};

// this must be extern in the main or in the .h file
USART_ManagerStruct usart1Manager={
	.moduleBase	= USART1_BASE,
	.init		= {
		.BaudRate = 9600,
		.WordLength = USART_WORDLENGTH_8B,
		.StopBits = USART_STOPBITS_1,
		.Parity = USART_PARITY_NONE,
		.Mode = USART_MODE_TX_RX

	}
	// .pTxBuffPtr	= NULL_PTR,
	// .TxXferSize	= 0,
	// .TxXferCount= 0,
	// .pRxBuffPtr	= NULL_PTR,
	// .RxXferSize	= 0,
	// .RxXferCount= 0,
	// .txState		= USART_STATE_RESET,
	// .rxState		= USART_STATE_RESET,
	// .ErrorCode	= 1
};

// usart1Manager.ErrorCode = 0;
