/*   setting
modes:
Synchronous
Asynchronous
..
*
basic parameters:
	Baud rate
	word length
	parity
	stop bits
*
*/

/*** Information about USART
 * USART sends the least significient bit first
 */

/*** Information about USART configuration
 * USART Instance is the usart base module
 *! THIS DRIVER HANDLES ONLY THE ASYNCRONOUS MODE
 */

#include "rcc.h"
#include "std_types.h"
#include "usart.h"
#include "usart_regs.h"
#include "common_macros.h"
#include "nvic.h"
#include "dma.h"

/************************************************************************************************************/
/********************************************** USART FUNCTIONS *********************************************/
/************************************************************************************************************/

/*---------------------------------------- USART Intialization function -------------------------------------------*/
void USART_Init(USART_ManagerStruct *usartxManager)
{

	switch ((uint32)usartxManager->Instance)
	{
	case USART1_BASE:
		RCC_Enable(RCC_USART1);
		break;

	case USART2_BASE:
		RCC_Enable(RCC_USART2);
		break;

	case USART6_BASE:
		RCC_Enable(RCC_USART6);
		break;

	default:
		break;
	}

	// disable the usart
	usartxManager->Instance->CR1 &= ~USART_CR1_UE;

	// Configure stop bits
	CLEAR_AND_SET_BYMASKS(usartxManager->Instance->CR2, USART_CR2_STOP_clrMsk, usartxManager->Init.StopBits);

	// configure Wordlength, parity, TxorRX mode
	CLEAR_AND_SET_BYMASKS(usartxManager->Instance->CR1,
							((uint32)(USART_CR1_M_clrMsk | USART_CR1_PCE_clrMsk | USART_CR1_PS_clrMsk | USART_CR1_TE_clrMsk | USART_CR1_RE_clrMsk)),
							((uint32)usartxManager->Init.WordLength | usartxManager->Init.Parity | usartxManager->Init.Mode));

	// configure baudrate
	usartxManager->Instance->BRR = UART_BRR_SAMPLING16(16000000, usartxManager->Init.BaudRate);

	// set usart states to ready
	usartxManager->txState = USART_STATE_READY;
	usartxManager->rxState = USART_STATE_READY;

	// enable the usart
	usartxManager->Instance->CR1 |= USART_CR1_UE;

	// reset the error code
	usartxManager->ErrorCode = USART_ERROR_NONE;

	return;
}


/*------------------------------------------ USART Polling functions ----------------------------------------------*/
void USART_sendByte_polling(USART_ManagerStruct *usartxManager, const uint8 data)
{
	/* 1. wait til the TDR is empty and ready to take-in data (wait for the TXE flag).
	 * 2. then write the data in the Data Register.
	 */

	// stop while the TX line is full  (wait while the TXE flag is clear)
	while (BIT_IS_CLEAR(usartxManager->Instance->SR, USART_SR_TXE_Pos))
	{
	}

	// writing the data in the data Register DR
	usartxManager->Instance->DR = data;

	// not nessecary    only nessecaery after the last byte sent by the uart to  indiacte that all is complete and its okay to disable the usart after it
	// while (!((USART_basePtr->SR)&(USART_SR_TC_Pos)))
}
uint8 USART_recieveByte_polling(USART_ManagerStruct *usartxManager)
{
	/* 1. wait for the module to detect a byte on the line. that will raise a flag (RXNE) RX Not Empty. wait for the flag.
	 * 2. then read and return the recieved data.
	 */

	// stop while the RX line is empty  (wait while the RXNE flag is clear)
	while (BIT_IS_CLEAR(usartxManager->Instance->SR, USART_SR_RXNE_Pos))
	{
	}

	// read and return the recieved data
	return usartxManager->Instance->DR;
}


/*-------------------------------------------- USART Interrupts functions --------------------------------------------*/
// Transmit functions by interrupts
MCALStatus_t USART_startTransmit_IT(USART_ManagerStruct *usartxManager, const uint8 *pData, uint16 Size)
{
	if (usartxManager->txState != USART_STATE_READY)
	{
		return MCAL_BUSY;
	}

	if ((pData == NULL) || (Size == 0U))
	{
		return MCAL_ERROR;
	}

	usartxManager->pTxBuffPtr = pData;
	usartxManager->TxXferSize = Size;
	usartxManager->TxXferCount = Size;

	// reset error state
	usartxManager->ErrorCode = USART_ERROR_NONE;

	// set busy state to indiacte that the USart is in use
	usartxManager->txState = USART_STATE_BUSY_TX;

	// enable interrupt TXE when the tx is empty and ready to write the new data
	usartxManager->Instance->CR1 |= USART_CR1_TXEIE;

	// enable USART module global interrupt
	switch((uint32)usartxManager->Instance){
		case USART1_BASE:
			__NVIC_EnableIRQ(USART1_IRQn);
			break;
		case USART2_BASE:
			__NVIC_EnableIRQ(USART2_IRQn);
			break;
		case USART6_BASE:
			__NVIC_EnableIRQ(USART6_IRQn);
			break;
		default:
			//WRONG INPUTS
			return MCAL_ERROR;
			break;
	}

	return MCAL_OK;
}
static MCALStatus_t USART_dataTransmit_IT(USART_ManagerStruct *usartxManager)
{
	const uint16_t *tmp;

	/* Check that a Tx process is ongoing */
	if (usartxManager->txState == USART_STATE_BUSY_TX)
	{
		if ((usartxManager->Init.WordLength == USART_WORDLENGTH_9B) && (usartxManager->Init.Parity == USART_PARITY_NONE))
		{
			tmp = (const uint16_t *)usartxManager->pTxBuffPtr;
			usartxManager->Instance->DR = (uint16_t)(*tmp & (uint16_t)0x01FF);
			usartxManager->pTxBuffPtr += 2U;
		}
		else
		{
			usartxManager->Instance->DR = (uint8_t)(*usartxManager->pTxBuffPtr++ & (uint8_t)0x00FF);
		}

		if (--(usartxManager->TxXferCount) == 0U)
		{
			/* Disable the UART Transmit Data Register Empty Interrupt */
			usartxManager->Instance->CR1 &= ~USART_CR1_TXEIE_clrMsk;

			/* Enable the UART Transmit Complete Interrupt */
			usartxManager->Instance->CR1 |= USART_CR1_TCIE;
		}
		return MCAL_OK;
	}
	else
	{
		return MCAL_BUSY;
	}
}
static void USART_EndTxTransfer(USART_ManagerStruct *usartxManager)
{
	/* Disable TXEIE and TCIE interrupts */
	ATOMIC_CLEAR_BIT(usartxManager->Instance->CR1, (USART_CR1_TXEIE | USART_CR1_TCIE));

	/* At end of Tx process, restore huart->gState to Ready */
	usartxManager->txState = USART_STATE_READY;

	/*maybe you can add a callbackfunction and call it*/
}

// Recieve functions by interrupts
MCALStatus_t USART_startReceive_IT(USART_ManagerStruct *usartxManager, uint8_t *pData, uint16_t Size)
{
	/* Check that a Rx process is not already ongoing */
	if (usartxManager->rxState != USART_STATE_READY)
	{
		return MCAL_BUSY;
	}

	if ((pData == NULL_PTR) || (Size == 0U))
	{
		return MCAL_ERROR;
	}

	/* Set Reception type to Standard reception */
	// usartxManager->ReceptionType = HAL_UART_RECEPTION_STANDARD;
	usartxManager->pRxBuffPtr = pData;
	usartxManager->RxXferSize = Size;
	usartxManager->RxXferCount = Size;

	// to reset the error state if you handle it in the manager
	usartxManager->ErrorCode = USART_ERROR_NONE;
	usartxManager->rxState = USART_STATE_BUSY_RX;

	// 111111111111111 enable ERROR interrupts
	if (usartxManager->Init.Parity != USART_PARITY_NONE)
	{
		/* Enable the UART Parity Error Interrupt */
		usartxManager->Instance->CR1 |= (USART_CR1_PEIE_Msk);
	}

	/* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
	usartxManager->Instance->CR1 |= (USART_CR3_EIE_Msk);
	// 111111111111111

	/* Enable the UART Data Register not empty Interrupt */
	usartxManager->Instance->CR1 |= (USART_CR1_RXNEIE_Msk);


	/* enable USART module global interrupt */
	switch((uint32)usartxManager->Instance){
		case USART1_BASE:
			__NVIC_EnableIRQ(USART1_IRQn);
			break;
		case USART2_BASE:
			__NVIC_EnableIRQ(USART2_IRQn);
			break;
		case USART6_BASE:
			__NVIC_EnableIRQ(USART6_IRQn);
			break;
		default:
			//WRONG INPUTS
			return MCAL_ERROR;
			break;
	}
	
	
	return MCAL_OK;
}
static MCALStatus_t USART_dataRecieve_IT(USART_ManagerStruct *usartxManager)
{
	uint8_t *pdata8bits;
	uint16_t *pdata16bits;

	/* Check that a Rx process is ongoing */
	if (usartxManager->rxState == USART_STATE_BUSY_RX)
	{
		if ((usartxManager->Init.WordLength == USART_WORDLENGTH_9B) && (usartxManager->Init.Parity == USART_PARITY_NONE))
		{
			pdata8bits = NULL;
			pdata16bits = (uint16_t *)usartxManager->pRxBuffPtr;
			*pdata16bits = (uint16_t)(usartxManager->Instance->DR & (uint16_t)0x01FF);
			usartxManager->pRxBuffPtr += 2U;
		}
		else
		{
			pdata8bits = (uint8_t *)usartxManager->pRxBuffPtr;
			pdata16bits = NULL;

			if ((usartxManager->Init.WordLength == USART_WORDLENGTH_9B) || ((usartxManager->Init.WordLength == USART_WORDLENGTH_8B) && (usartxManager->Init.Parity == USART_PARITY_NONE)))
			{
				*pdata8bits = (uint8_t)(usartxManager->Instance->DR & (uint8_t)0x00FF);
			}
			else
			{
				*pdata8bits = (uint8_t)(usartxManager->Instance->DR & (uint8_t)0x007F);
			}
			usartxManager->pRxBuffPtr += 1U;
		}

		if (--usartxManager->RxXferCount == 0U)
		{
			/* Disable the UART Data Register not empty Interrupt */
			usartxManager->Instance->CR1 &= ~(USART_CR1_RXNEIE_clrMsk);

			// 11111111111111111 Disable Error Interrupts
			/* Disable the UART Parity Error Interrupt */
			usartxManager->Instance->CR1 &= ~(USART_CR1_PEIE_clrMsk);

			/* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
			usartxManager->Instance->CR1 &= ~(USART_CR3_EIE_clrMsk);
			// 11111111111111111

			/* Rx process is completed, restore usartxManager->RxState to Ready */
			usartxManager->rxState = USART_STATE_READY;

			//   /* Initialize type of RxEvent to Transfer Complete */
			//   usartxManager->RxEventType = HAL_UART_RXEVENT_TC;

			// /* Check current reception Mode :
			// 	If Reception till IDLE event has been selected : */
			// if (usartxManager->ReceptionType == HAL_UART_RECEPTION_TOIDLE)
			// {
			// 	/* Set reception type to Standard */
			// 	usartxManager->ReceptionType = HAL_UART_RECEPTION_STANDARD;

			// 	/* Disable IDLE interrupt */
			// 	ATOMIC_CLEAR_BIT(usartxManager->Instance->CR1, USART_CR1_IDLEIE);

			// 	/* Check if IDLE flag is set */
			// 	if (__HAL_UART_GET_FLAG(usartxManager, UART_FLAG_IDLE))
			// 	{
			// 		/* Clear IDLE flag in ISR */
			// 		__HAL_UART_CLEAR_IDLEFLAG(usartxManager);
			// 	}

			// }

			return MCAL_OK;
		}

		return MCAL_OK;
	}
	else
	{
		return MCAL_BUSY;
	}
}
static void USART_EndRxTransfer(USART_ManagerStruct *usartxManager)
{
	/* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
	//   ATOMIC_CLEAR_BIT(usartxManager->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));
	//   ATOMIC_CLEAR_BIT(usartxManager->Instance->CR3, USART_CR3_EIE);

	Disable_Interrupts();
	usartxManager->Instance->CR1 &= ~(USART_CR1_RXNEIE_clrMsk | USART_CR1_PEIE_clrMsk);
	usartxManager->Instance->CR3 &= ~(USART_CR3_EIE);
	Enable_Interrupts();

	/* In case of reception waiting for IDLE event, disable also the IDLE IE interrupt source */
	//   if (usartxManager->ReceptionType == USART_RECEPTION_TOIDLE)
	//   {
	// 		Disable_Interrupts();
	//=		ATOMIC_CLEAR_BIT(usartxManager->Instance->CR1, USART_CR1_IDLEIE);
	//		Enable_Interrupts();
	//   }

	/* At end of Rx process, restore huart->RxState to Ready */
	usartxManager->rxState = USART_STATE_READY;
	//   usartxManager->ReceptionType = HAL_UART_RECEPTION_STANDARD;
}





/***************************************************************************************************************************/
/**************************************************** USART using DMA ******************************************************/
/***************************************************************************************************************************/

MCALStatus_t USART_Transmit_DMA(USART_ManagerStruct *huart, const uint8_t *pData, uint16_t Size)
{
	// const uint32_t *tmp;

	/* Check that a Tx process is not already ongoing */
	if (huart->txState != USART_STATE_READY)
	{
		return MCAL_BUSY;
	}

	if ((pData == NULL) || (Size == 0U))
	{
		return MCAL_ERROR;
	}

	huart->pTxBuffPtr = pData;
	huart->TxXferSize = Size;
	huart->TxXferCount = Size;

	huart->ErrorCode = USART_ERROR_NONE;
	huart->txState = USART_STATE_BUSY_TX;

	// /* Set the UART DMA transfer complete callback */
	// huart->hdmatx->XferCpltCallback = UART_DMATransmitCplt;

	// /* Set the UART DMA Half transfer complete callback */
	// huart->hdmatx->XferHalfCpltCallback = UART_DMATxHalfCplt;

	// /* Set the DMA error callback */
	// huart->hdmatx->XferErrorCallback = UART_DMAError;

	// /* Set the DMA abort callback */
	// huart->hdmatx->XferAbortCallback = NULL;

	/* Enable the UART transmit DMA stream */
	DMA_Start_IT(huart->hdmatx, (uint32)((uint8*)pData+1), (uint32_t)&huart->Instance->DR, Size-1);

	/* Clear the TC flag in the USART SR register by writing 0 to it */
	(huart)->Instance->SR = ~(UART_FLAG_TC);

	/* Enable the DMA transfer for transmit request by setting the DMAT bit
		in the UART CR3 register */
	ATOMIC_SET_BIT(huart->Instance->CR3, USART_CR3_DMAT);

	/* Send the first Byte of the dma to start the dma */
	if ((huart->Instance->SR & USART_SR_TXE)){
		huart->Instance->DR = *(uint8*)pData;
	}else {
		return MCAL_ERROR;
	}

	return MCAL_OK;
}

MCALStatus_t USART_Receive_DMA(USART_ManagerStruct *huart, uint8_t *pData, uint16_t Size)
{
	/* Check that a Rx process is not already ongoing */
	if (huart->rxState != USART_STATE_READY)
	{
		return MCAL_BUSY;
	}
	if ((pData == NULL) || (Size == 0U))
	{
		return MCAL_ERROR;
	}

	/* Set Reception type to Standard reception */
	// huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;

	huart->pRxBuffPtr = pData;
	huart->RxXferSize = Size;

	huart->ErrorCode = USART_ERROR_NONE;
	huart->rxState = USART_STATE_BUSY_RX;

	/*<! mda call back functions >*/
	// /* Set the UART DMA transfer complete callback */
	// huart->hdmarx->XferCpltCallback = UART_DMAReceiveCplt;

	// /* Set the UART DMA Half transfer complete callback */
	// huart->hdmarx->XferHalfCpltCallback = UART_DMARxHalfCplt;

	// /* Set the DMA error callback */
	// huart->hdmarx->XferErrorCallback = UART_DMAError;

	// /* Set the DMA abort callback */
	// huart->hdmarx->XferAbortCallback = NULL;

	/* Enable the DMA stream */
	DMA_Start_IT(huart->hdmarx, (uint32_t)&huart->Instance->DR, (uint32)pData, Size);

	/* Clear the usart Overrun flag just before enabling the DMA Rx request: can be mandatory for the second transfer */
	// Clear USART Overrun error
	{
		__IO uint32_t tmpreg = 0x00U;
		tmpreg = huart->Instance->SR;
		tmpreg = huart->Instance->DR;
		(void)tmpreg;
	}

	if (huart->Init.Parity != USART_PARITY_NONE)
	{
		/* Enable the UART Parity Error Interrupt */
		ATOMIC_SET_BIT(huart->Instance->CR1, USART_CR1_PEIE);
	}

	/* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
	ATOMIC_SET_BIT(huart->Instance->CR3, USART_CR3_EIE);

	/* Enable the DMA transfer for the receiver request by setting the DMAR bit
	in the UART CR3 register */
	ATOMIC_SET_BIT(huart->Instance->CR3, USART_CR3_DMAR);

	return MCAL_OK;
}





/*--------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------- DMA Interrupt Handler Function ---------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------*/
/** USART Interrupt Handler Function ************************************************************/
enum FlagStatus
{
	RESET = 0U,
	SET = !RESET
};
void MCAL_USART_IRQHandler(USART_ManagerStruct *usartxManager)
{
	uint32_t isrflags = (usartxManager->Instance->SR);
	uint32_t cr1its = (usartxManager->Instance->CR1);
	uint32_t cr3its = (usartxManager->Instance->CR3);
	uint32_t errorflags = 0x00U;
	uint32_t dmarequest = 0x00U;

	errorflags = (isrflags & (uint32_t)(USART_SR_ORE | USART_SR_NE | USART_SR_FE | USART_SR_PE));

	/* receive if no error and in recieve mode */
	if (errorflags == RESET)
	{
		/* UART in mode Receiver -------------------------------------------------*/
		if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
		{
			USART_dataRecieve_IT(usartxManager);
			return; // no error and recieve mode receives
		}
	}

	// handle error if its specific error interrupt is enabled
	/* If some errors occur note them in The ErrorCode. then Receive and Abort (cancel all recive interrupts. and fn. and return)*/
	if ((errorflags != RESET) && (((cr3its & USART_CR3_EIE) != RESET) || ((cr1its & (USART_CR1_RXNEIE | USART_CR1_PEIE)) != RESET)))
	{
		/******************** SET ERROR CODE ********************/
		/* UART parity error interrupt occurred ----------------------------------*/
		if (((isrflags & USART_SR_PE) != RESET) && ((cr1its & USART_CR1_PEIE) != RESET))
		{
			usartxManager->ErrorCode |= USART_ERROR_PE;
		}

		/* UART frame error flag is raised ------------------------------------*/
		if (((isrflags & USART_SR_FE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))
		{
			usartxManager->ErrorCode |= USART_ERROR_FE;
		}

		/* UART noise error flag is raised --*/ // the noise flag doesn't generate interrupt. it is raised at receiving as same time as RXNE interrupt.
		if (((isrflags & USART_SR_NE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))
		{
			usartxManager->ErrorCode |= USART_ERROR_NE;
		}

		/* UART Over-Run interrupt occurred --------------------------------------*/
		if (((isrflags & USART_SR_ORE) != RESET) && (((cr1its & USART_CR1_RXNEIE) != RESET) || ((cr3its & USART_CR3_EIE) != RESET)))
		{
			usartxManager->ErrorCode |= USART_ERROR_ORE;
		}
		/******************************************************/

		/* Call UART Error Call back function if need to --------------------------*/
		// if there is an error receive the remaining data in DR and then ABORT
		if (usartxManager->ErrorCode != USART_ERROR_NONE)
		{
			/* Receive if UART in mode Receiver -----------------------------------------------*/
			if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
			{
				USART_dataRecieve_IT(usartxManager);
			}

			/* If Overrun error occurs, or if any error occurs in DMA mode reception, consider error as blocking (ABORT) */
			dmarequest = (usartxManager->Instance->CR3 & USART_CR3_DMAR);
			if (((usartxManager->ErrorCode & USART_ERROR_ORE) != RESET) || dmarequest)
			{
				/* Blocking error : transfer is aborted
				   Set the UART state ready to be able to start again the process,
				   Disable Rx Interrupts, and disable Rx DMA request, if ongoing */
				USART_EndRxTransfer(usartxManager);

				/* Disable the UART DMA Rx request mode if enabled */
				if (dmarequest)
				{
					ATOMIC_CLEAR_BIT(usartxManager->Instance->CR3, USART_CR3_DMAR);

					/* Abort the UART DMA Rx stream */
					if (usartxManager->hdmarx != NULL)
					{
						/* Set the UART DMA Abort callback :
						   will lead to call HAL_UART_ErrorCallback() at end of DMA abort procedure */
						// usartxManager->hdmarx->XferAbortCallback = UART_DMAAbortOnError;
						if (DMA_Abort_IT(usartxManager->hdmarx) != MCAL_OK) // dma abort
						{
							/* Call Directly XferAbortCallback function in case of error */
							// usartxManager->hdmarx->XferAbortCallback(usartxManager->hdmarx);
						}
					}
					else
					{
						/* Call user USART error callback */
					}
				}
				else
				{
					/* Call user USART error callback */
				}
			}
			else // if not overrun error
			{
				/* Non Blocking error : transfer could go on.
				   Error is notified to user through user error callback */

				usartxManager->ErrorCode = USART_ERROR_NONE;
			}
		}
		return;
	} /* End if some error occurs */



	// idle
	{
	// /* Check current reception Mode :
	//    If Reception till IDLE event has been selected : */
	// if ((usartxManager->ReceptionType == HAL_UART_RECEPTION_TOIDLE) && ((isrflags & USART_SR_IDLE) != 0U) && ((cr1its & USART_CR1_IDLEIE) != 0U))
	// {
	// 	__HAL_UART_CLEAR_IDLEFLAG(usartxManager);

	// 	/* Check if DMA mode is enabled in UART */
	// 	if (HAL_IS_BIT_SET(usartxManager->Instance->CR3, USART_CR3_DMAR))
	// 	{
	// 		/* DMA mode enabled */
	// 		/* Check received length : If all expected data are received, do nothing,
	// 		   (DMA cplt callback will be called).
	// 		   Otherwise, if at least one data has already been received, IDLE event is to be notified to user */
	// 		uint16_t nb_remaining_rx_data = (uint16_t)__HAL_DMA_GET_COUNTER(usartxManager->hdmarx);
	// 		if ((nb_remaining_rx_data > 0U) && (nb_remaining_rx_data < usartxManager->RxXferSize))
	// 		{
	// 			/* Reception is not complete */
	// 			usartxManager->RxXferCount = nb_remaining_rx_data;

	// 			/* In Normal mode, end DMA xfer and HAL UART Rx process*/
	// 			if (usartxManager->hdmarx->Init.Mode != DMA_CIRCULAR)
	// 			{
	// 				/* Disable PE and ERR (Frame error, noise error, overrun error) interrupts */
	// 				ATOMIC_CLEAR_BIT(usartxManager->Instance->CR1, USART_CR1_PEIE);
	// 				ATOMIC_CLEAR_BIT(usartxManager->Instance->CR3, USART_CR3_EIE);

	// 				/* Disable the DMA transfer for the receiver request by resetting the DMAR bit
	// 				   in the UART CR3 register */
	// 				ATOMIC_CLEAR_BIT(usartxManager->Instance->CR3, USART_CR3_DMAR);

	// 				/* At end of Rx process, restore usartxManager->RxState to Ready */
	// 				usartxManager->rxState = USART_STATE_READY;
	// 				usartxManager->ReceptionType = HAL_UART_RECEPTION_STANDARD;

	// 				ATOMIC_CLEAR_BIT(usartxManager->Instance->CR1, USART_CR1_IDLEIE);

	// 				/* Last bytes received, so no need as the abort is immediate */
	// 				(void)HAL_DMA_Abort(usartxManager->hdmarx);
	// 			}

	// 			/* Initialize type of RxEvent that correspond to RxEvent callback execution;
	// 			In this case, Rx Event type is Idle Event */
	// 			usartxManager->RxEventType = HAL_UART_RXEVENT_IDLE;

	// 			///////////////if you want to add a cbf on event you can add it here
	// 		}
	// 	}
	// 	else
	// 	{
	// 		/* DMA mode not enabled */
	// 		/* Check received length : If all expected data are received, do nothing.
	// 		   Otherwise, if at least one data has already been received, IDLE event is to be notified to user */
	// 		uint16_t nb_rx_data = usartxManager->RxXferSize - usartxManager->RxXferCount;
	// 		if ((usartxManager->RxXferCount > 0U) && (nb_rx_data > 0U)) // ebtada w lessa fadlo bytes lw 7asl kda fa abort
	// 		{
	// 			//////////////////////////
	// 			/* Disable the UART Parity Error Interrupt and RXNE interrupts */
	// 			ATOMIC_CLEAR_BIT(usartxManager->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));

	// 			/* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
	// 			ATOMIC_CLEAR_BIT(usartxManager->Instance->CR3, USART_CR3_EIE);
	// 			//////////////////////////

	// 			/* Rx process is completed, restore usartxManager->RxState to Ready */
	// 			usartxManager->rxState = USART_STATE_READY;
	// 			usartxManager->ReceptionType = HAL_UART_RECEPTION_STANDARD;

	// 			ATOMIC_CLEAR_BIT(usartxManager->Instance->CR1, USART_CR1_IDLEIE);

	// 			/* Initialize type of RxEvent that correspond to RxEvent callback execution;
	// 			   In this case, Rx Event type is Idle Event */
	// 			usartxManager->RxEventType = HAL_UART_RXEVENT_IDLE;
	// 		}

	// 	}




	// 	return;
	// }
	;}





	/* UART in mode Transmitter ------------------------------------------------*/
	if (((isrflags & USART_SR_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET))
	{
		USART_dataTransmit_IT(usartxManager);
		return;
	}

	/* UART in mode Transmitter end --------------------------------------------*/
	if (((isrflags & USART_SR_TC) != RESET) && ((cr1its & USART_CR1_TCIE) != RESET))
	{
		USART_EndTxTransfer(usartxManager);
		return;
	}
}

