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

#include "rcc.h"
#include "std_types.h"
#include "usart.h"
#include "usart_regs.h"
#include "common_macros.h"
#include "nvic.h"
#include "dma.h"

/*****************************************************************************************/
/*********************************** USART FUNCTIONS *************************************/
/*****************************************************************************************/

/** USART initialize function ******************************************************************/
void USART_Init(USART_ManagerStruct *usartxManger)
{

	switch ((uint32)usartxManger->moduleBase)
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

	// Configure stop bits
	// USART_basePtr->CR2 = (USART_basePtr->CR2 & ~(USART_CR2_STOP_ClrMsk)) | StopBits;
	REG_CLEARANDSET_BYMASKS(usartxManger->moduleBase->CR2, USART_CR2_STOP_clrMsk, usartxManger->init.StopBits);

	// configure Wordlength, parity, TxorRX mode
	REG_CLEARANDSET_BYMASKS(usartxManger->moduleBase->CR1,
							((uint32)(		USART_CR1_M_clrMsk 	| USART_CR1_PCE_clrMsk | USART_CR1_PS_clrMsk | USART_CR1_TE_clrMsk | USART_CR1_RE_clrMsk)),
							((uint32)usartxManger->init.WordLength 	| 		usartxManger->init.Parity	 	|	 usartxManger->init.Mode));

	// configure baudrate
	usartxManger->moduleBase->BRR = UART_BRR_SAMPLING16(16000000, usartxManger->init.BaudRate);

	//set usart states to ready
	usartxManger->txState = USART_STATE_READY;
	usartxManger->rxState = USART_STATE_READY;

	// enable the usart
	(usartxManger->moduleBase->CR1 |= USART_CR1_UE);

	return;
}




/** USART functions that uses the interrupts ***************************************************/
// Transmit functions by interrupts
MCALStatus_t USART_startTransmit_IT(USART_ManagerStruct *usartxManger, const uint8 *pData, uint16 Size)
{
	if (usartxManger->txState != USART_STATE_READY)
	{
		return MCAL_BUSY;
	}

	if ((pData == NULL) || (Size == 0U))
	{
		return MCAL_ERROR;
	}

	usartxManger->pTxBuffPtr = pData;
	usartxManger->TxXferSize = Size;
	usartxManger->TxXferCount = Size;

	// set busy state to indiacte that the USart is in use
	usartxManger->txState = USART_STATE_BUSY_TX;

	// enable interrupt TXE when the tx is empty and ready to write the new data
	usartxManger->moduleBase->CR1 |= USART_CR1_TXEIE;

	Enable_Interrupts();
	__NVIC_EnableIRQ(USART1_IRQn);

	return MCAL_OK;
}

static MCALStatus_t USART_TransmitData_IT(USART_ManagerStruct *usartxManger)
{
	const uint16_t *tmp;

	/* Check that a Tx process is ongoing */
	if (usartxManger->txState == USART_STATE_BUSY_TX)
	{
		if ((usartxManger->init.WordLength == USART_WORDLENGTH_9B) && (usartxManger->init.Parity == USART_PARITY_NONE))
		{
			tmp = (const uint16_t *)usartxManger->pTxBuffPtr;
			usartxManger->moduleBase->DR = (uint16_t)(*tmp & (uint16_t)0x01FF);
			usartxManger->pTxBuffPtr += 2U;
		}
		else
		{
			usartxManger->moduleBase->DR = (uint8_t)(*usartxManger->pTxBuffPtr++ & (uint8_t)0x00FF);
		}

		if (--(usartxManger->TxXferCount) == 0U)
		{
			/* Disable the UART Transmit Data Register Empty Interrupt */
			usartxManger->moduleBase->CR1 &= ~USART_CR1_TXEIE_clrMsk;

			/* Enable the UART Transmit Complete Interrupt */
			usartxManger->moduleBase->CR1 |= USART_CR1_TCIE;
		}
		return MCAL_OK;
	}
	else
	{
		return MCAL_BUSY;
	}
}

static MCALStatus_t USART_endTransmit_IT(USART_ManagerStruct *usartxManger)
{
	/* Disable the UART Transmit Complete Interrupt */
	usartxManger->moduleBase->CR1 &= ~USART_CR1_TCIE_clrMsk;

	/* Tx process is ended, restore huart->gState to Ready */
	usartxManger->txState = USART_STATE_READY;

	return MCAL_OK;
}

// Recieve functions by interrupts
MCALStatus_t USART_startReceive_IT(USART_ManagerStruct *usartxManger, uint8_t *pData, uint16_t Size)
{
	/* Check that a Rx process is not already ongoing */
	if (usartxManger->rxState == USART_STATE_READY)
	{
		if ((pData == NULL_PTR) || (Size == 0U))
		{
			return MCAL_ERROR;
		}

		/* Set Reception type to Standard reception */
		// usartxManger->ReceptionType = HAL_UART_RECEPTION_STANDARD;

		usartxManger->pRxBuffPtr = pData;
		usartxManger->RxXferSize = Size;
		usartxManger->RxXferCount = Size;

		// to reset the error state if you handle it in the manager
		//   usartxManger->ErrorCode = USART_ERROR_NONE;
		usartxManger->rxState = USART_STATE_BUSY_RX;

		// 111111111111111 enable ERROR interrupts
		if (usartxManger->init.Parity != USART_PARITY_NONE)
		{
			/* Enable the UART Parity Error Interrupt */
			usartxManger->moduleBase->CR1 |= (USART_CR1_PEIE_Msk);
		}

		/* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
		usartxManger->moduleBase->CR1 |= (USART_CR3_EIE_Msk);
		// 111111111111111

		/* Enable the UART Data Register not empty Interrupt */
		usartxManger->moduleBase->CR1 |= (USART_CR1_RXNEIE_Msk);

		/* enable usart global interrupt*/
		__NVIC_EnableIRQ(USART1_IRQn);//usart1 you need to be be hadle with all usarts

		return MCAL_OK;
	}
	else
	{
		return MCAL_BUSY;
	}
}

static MCALStatus_t USART_ReceiveData_IT(USART_ManagerStruct *usartxManger)
{
	uint8_t *pdata8bits;
	uint16_t *pdata16bits;

	/* Check that a Rx process is ongoing */
	if (usartxManger->rxState == USART_STATE_BUSY_RX)
	{
		if ((usartxManger->init.WordLength == USART_WORDLENGTH_9B) && (usartxManger->init.Parity == USART_PARITY_NONE))
		{
			pdata8bits = NULL;
			pdata16bits = (uint16_t *)usartxManger->pRxBuffPtr;
			*pdata16bits = (uint16_t)(usartxManger->moduleBase->DR & (uint16_t)0x01FF);
			usartxManger->pRxBuffPtr += 2U;
		}
		else
		{
			pdata8bits = (uint8_t *)usartxManger->pRxBuffPtr;
			pdata16bits = NULL;

			if ((usartxManger->init.WordLength == USART_WORDLENGTH_9B) || ((usartxManger->init.WordLength == USART_WORDLENGTH_8B) && (usartxManger->init.Parity == USART_PARITY_NONE)))
			{
				*pdata8bits = (uint8_t)(usartxManger->moduleBase->DR & (uint8_t)0x00FF);
			}
			else
			{
				*pdata8bits = (uint8_t)(usartxManger->moduleBase->DR & (uint8_t)0x007F);
			}
			usartxManger->pRxBuffPtr += 1U;
		}

		if (--usartxManger->RxXferCount == 0U)
		{
			/* Disable the UART Data Register not empty Interrupt */
			usartxManger->moduleBase->CR1 &= ~(USART_CR1_RXNEIE_clrMsk);

			// 11111111111111111 Disable Error Interrupts
			/* Disable the UART Parity Error Interrupt */
			usartxManger->moduleBase->CR1 &= ~(USART_CR1_PEIE_clrMsk);

			/* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
			usartxManger->moduleBase->CR1 &= ~(USART_CR3_EIE_clrMsk);
			// 11111111111111111

			/* Rx process is completed, restore usartxManger->RxState to Ready */
			usartxManger->rxState = USART_STATE_READY;

			//   /* Initialize type of RxEvent to Transfer Complete */
			//   usartxManger->RxEventType = HAL_UART_RXEVENT_TC;

			// /* Check current reception Mode :
			// 	If Reception till IDLE event has been selected : */
			// if (usartxManger->ReceptionType == HAL_UART_RECEPTION_TOIDLE)
			// {
			// 	/* Set reception type to Standard */
			// 	usartxManger->ReceptionType = HAL_UART_RECEPTION_STANDARD;

			// 	/* Disable IDLE interrupt */
			// 	ATOMIC_CLEAR_BIT(usartxManger->moduleBase->CR1, USART_CR1_IDLEIE);

			// 	/* Check if IDLE flag is set */
			// 	if (__HAL_UART_GET_FLAG(usartxManger, UART_FLAG_IDLE))
			// 	{
			// 		/* Clear IDLE flag in ISR */
			// 		__HAL_UART_CLEAR_IDLEFLAG(usartxManger);
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

static void UART_EndRxTransfer(USART_ManagerStruct *usartxManger)
{
	/* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
	//   ATOMIC_CLEAR_BIT(usartxManger->moduleBase->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));
	//   ATOMIC_CLEAR_BIT(usartxManger->moduleBase->CR3, USART_CR3_EIE);

	Disable_Interrupts();
	usartxManger->moduleBase->CR1 &= ~(USART_CR1_RXNEIE_clrMsk | USART_CR1_PEIE_clrMsk);
	usartxManger->moduleBase->CR3 &= ~(USART_CR3_EIE);
	Enable_Interrupts();

	/* In case of reception waiting for IDLE event, disable also the IDLE IE interrupt source */
	//   if (usartxManger->ReceptionType == USART_RECEPTION_TOIDLE)
	//   {
	// 		Disable_Interrupts();
	//=		ATOMIC_CLEAR_BIT(usartxManger->Instance->CR1, USART_CR1_IDLEIE);
	//		Enable_Interrupts();
	//   }

	/* At end of Rx process, restore huart->RxState to Ready */
	usartxManger->rxState = USART_STATE_READY;
	//   usartxManger->ReceptionType = HAL_UART_RECEPTION_STANDARD;
}




/** USART functions that uses polling ***********************************************************/
void USART_sendByte_polling(USART_ManagerStruct *usartxManger, const uint8 data)
{
	/* 1. wait til the TDR is empty and ready to take-in data (wait for the TXE flag).
	 * 2. then write the data in the Data Register.
	 */

	// stop while the TX line is full  (wait while the TXE flag is clear)
	while (BIT_IS_CLEAR(usartxManger->moduleBase->SR, USART_SR_TXE_Pos))
	{
	}

	// writing the data in the data Register DR
	usartxManger->moduleBase->DR = data;

	// not nessecary    only nessecaery after the last byte sent by the uart to  indiacte that all is complete and its okay to disable the usart after it
	// while (!((USART_basePtr->SR)&(USART_SR_TC_Pos)))
}

uint8 USART_recieveByte_polling(USART_ManagerStruct *usartxManger)
{
	/* 1. wait for the module to detect a byte on the line. that will raise a flag (RXNE) RX Not Empty. wait for the flag.
	 * 2. then read and return the recieved data.
	 */

	// stop while the RX line is empty  (wait while the RXNE flag is clear)
	while (BIT_IS_CLEAR(usartxManger->moduleBase->SR, USART_SR_RXNE_Pos))
	{
	}

	// read and return the recieved data
	return usartxManger->moduleBase->DR;
}




/** USART Interrupt Handler Function ************************************************************/

enum FlagStatus{
	RESET = 0U,
	SET = !RESET
};

void MCAL_USART_IRQHandler(USART_ManagerStruct *usartxManger)
{
	uint32_t isrflags = (usartxManger->moduleBase->SR);
	uint32_t cr1its = (usartxManger->moduleBase->CR1);
	uint32_t cr3its = (usartxManger->moduleBase->CR3);
	uint32_t errorflags = 0x00U;
	// uint32_t dmarequest = 0x00U;


	errorflags = (isrflags & (uint32_t)(USART_SR_ORE | USART_SR_NE | USART_SR_FE | USART_SR_PE));
	//receive mode and no error
	/* If no error occurs, receive*/
	if (errorflags == RESET)
	{
		/* UART in mode Receiver -------------------------------------------------*/
		if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET)){
			USART_ReceiveData_IT(usartxManger);
			return;	//no error and recieve mode receives
		}
	}

	//error
	/* If some errors occur Abort (cancel all recive it. and fn. and return)*/
	if ((errorflags != RESET) && (((cr3its & USART_CR3_EIE) != RESET) || ((cr1its & (USART_CR1_RXNEIE | USART_CR1_PEIE)) != RESET)))
	{
		/* UART parity error interrupt occurred ----------------------------------*/
		if (((isrflags & USART_SR_PE) != RESET) && ((cr1its & USART_CR1_PEIE) != RESET)){
			usartxManger->ErrorCode |= USART_ERROR_PE;
		}

		/* UART frame error flag is raised ------------------------------------*/
		if (((isrflags & USART_SR_FE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET)){
			usartxManger->ErrorCode |= USART_ERROR_FE;
		}

		/* UART noise error flag is raised --*/ //the noise flag doesn't generate interrupt. it is raised at receiving as same time as RXNE interrupt.
		if (((isrflags & USART_SR_NE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET)){
			usartxManger->ErrorCode |= USART_ERROR_NE;
		}

		/* UART Over-Run interrupt occurred --------------------------------------*/
		if (((isrflags & USART_SR_ORE) != RESET) && (((cr1its & USART_CR1_RXNEIE) != RESET) || ((cr3its & USART_CR3_EIE) != RESET))){
			usartxManger->ErrorCode |= USART_ERROR_ORE;
		}

		/* Call UART Error Call back function if need be --------------------------*/
		// if there is an error and its interrupt is enabled   receive if there is data in DR and then ABORT
		if (usartxManger->ErrorCode != USART_ERROR_NONE)
		{
			/* UART in mode Receiver -----------------------------------------------*/
			if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
			{
				USART_ReceiveData_IT(usartxManger);
			}

			// /* If Overrun error occurs, or if any error occurs in DMA mode reception,
			//    consider error as blocking */
			// dmarequest = HAL_IS_BIT_SET(usartxManger->moduleBase->CR3, USART_CR3_DMAR);
			// if (((usartxManger->ErrorCode & USART_ERROR_ORE) != RESET) || dmarequest)
			// {
			// 	/* Blocking error : transfer is aborted
			// 	   Set the UART state ready to be able to start again the process,
			// 	   Disable Rx Interrupts, and disable Rx DMA request, if ongoing */
			// 	UART_EndRxTransfer(usartxManger);

			// 	/* Disable the UART DMA Rx request if enabled */
			// 	if (HAL_IS_BIT_SET(usartxManger->moduleBase->CR3, USART_CR3_DMAR))
			// 	{
			// 		ATOMIC_CLEAR_BIT(usartxManger->moduleBase->CR3, USART_CR3_DMAR);

			// 		/* Abort the UART DMA Rx stream */
			// 		if (usartxManger->hdmarx != NULL)
			// 		{
			// 			/* Set the UART DMA Abort callback :
			// 			   will lead to call HAL_UART_ErrorCallback() at end of DMA abort procedure */
			// 			usartxManger->hdmarx->XferAbortCallback = UART_DMAAbortOnError;
			// 			if (HAL_DMA_Abort_IT(usartxManger->hdmarx) != HAL_OK)
			// 			{
			// 				/* Call Directly XferAbortCallback function in case of error */
			// 				usartxManger->hdmarx->XferAbortCallback(usartxManger->hdmarx);
			// 			}
			// 		}
			// 		else
			// 		{
			// 			/* Call user error callback */
			// 		}
			// 	}
			// 	else
			// 	{
			// 		/* Call user error callback */
			// 	}
			// }
			// else
			{
				/* Non Blocking error : transfer could go on.
				   Error is notified to user through user error callback */

				usartxManger->ErrorCode = USART_ERROR_NONE;
			}
		}
		return;
	} /* End if some error occurs */

	// 	/* Check current reception Mode :
	// 	   If Reception till IDLE event has been selected : */
	// 	if ((usartxManger->ReceptionType == HAL_UART_RECEPTION_TOIDLE) && ((isrflags & USART_SR_IDLE) != 0U) && ((cr1its & USART_SR_IDLE) != 0U))
	// 	{
	// 		__HAL_UART_CLEAR_IDLEFLAG(usartxManger);

	// 		/* Check if DMA mode is enabled in UART */
	// 		if (HAL_IS_BIT_SET(usartxManger->moduleBase->CR3, USART_CR3_DMAR))
	// 		{
	// 			/* DMA mode enabled */
	// 			/* Check received length : If all expected data are received, do nothing,
	// 			   (DMA cplt callback will be called).
	// 			   Otherwise, if at least one data has already been received, IDLE event is to be notified to user */
	// 			uint16_t nb_remaining_rx_data = (uint16_t)__HAL_DMA_GET_COUNTER(usartxManger->hdmarx);
	// 			if ((nb_remaining_rx_data > 0U) && (nb_remaining_rx_data < usartxManger->RxXferSize))
	// 			{
	// 				/* Reception is not complete */
	// 				usartxManger->RxXferCount = nb_remaining_rx_data;

	// 				/* In Normal mode, end DMA xfer and HAL UART Rx process*/
	// 				if (usartxManger->hdmarx->Init.Mode != DMA_CIRCULAR)
	// 				{
	// 					/* Disable PE and ERR (Frame error, noise error, overrun error) interrupts */
	// 					ATOMIC_CLEAR_BIT(usartxManger->moduleBase->CR1, USART_CR1_PEIE);
	// 					ATOMIC_CLEAR_BIT(usartxManger->moduleBase->CR3, USART_CR3_EIE);

	// 					/* Disable the DMA transfer for the receiver request by resetting the DMAR bit
	// 					   in the UART CR3 register */
	// 					ATOMIC_CLEAR_BIT(usartxManger->moduleBase->CR3, USART_CR3_DMAR);

	// 					/* At end of Rx process, restore usartxManger->RxState to Ready */
	// 					usartxManger->rxState = USART_STATE_READY;
	// 					usartxManger->ReceptionType = HAL_UART_RECEPTION_STANDARD;

	// 					ATOMIC_CLEAR_BIT(usartxManger->moduleBase->CR1, USART_CR1_IDLEIE);

	// 					/* Last bytes received, so no need as the abort is immediate */
	// 					(void)HAL_DMA_Abort(usartxManger->hdmarx);
	// 				}

	// 				/* Initialize type of RxEvent that correspond to RxEvent callback execution;
	// 				In this case, Rx Event type is Idle Event */
	// 				usartxManger->RxEventType = HAL_UART_RXEVENT_IDLE;

	// 			///////////////if you want to add a cbf on event you can add it here
	// 			}
	// 			return;
	// 		}
	// 		else
	// 		{
	// 			/* DMA mode not enabled */
	// 			/* Check received length : If all expected data are received, do nothing.
	// 			   Otherwise, if at least one data has already been received, IDLE event is to be notified to user */
	// 			uint16_t nb_rx_data = usartxManger->RxXferSize - usartxManger->RxXferCount;
	// 			if ((usartxManger->RxXferCount > 0U) && (nb_rx_data > 0U))
	// 			{
	// //////////////////////////
	// 				/* Disable the UART Parity Error Interrupt and RXNE interrupts */
	// 				ATOMIC_CLEAR_BIT(usartxManger->moduleBase->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));

	// 				/* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
	// 				ATOMIC_CLEAR_BIT(usartxManger->moduleBase->CR3, USART_CR3_EIE);
	// //////////////////////////

	// 				/* Rx process is completed, restore usartxManger->RxState to Ready */
	// 				usartxManger->rxState = USART_STATE_READY;
	// 				usartxManger->ReceptionType = HAL_UART_RECEPTION_STANDARD;

	// 				ATOMIC_CLEAR_BIT(usartxManger->moduleBase->CR1, USART_CR1_IDLEIE);

	// 				/* Initialize type of RxEvent that correspond to RxEvent callback execution;
	// 				   In this case, Rx Event type is Idle Event */
	// 				usartxManger->RxEventType = HAL_UART_RXEVENT_IDLE;
	// 			}
	// 			return;
	// 		}
	// }

	/* UART in mode Transmitter ------------------------------------------------*/
	if (((isrflags & USART_SR_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET))
	{
		USART_TransmitData_IT(usartxManger);
		return;
	}

	/* UART in mode Transmitter end --------------------------------------------*/
	if (((isrflags & USART_SR_TC) != RESET) && ((cr1its & USART_CR1_TCIE) != RESET))
	{
		USART_endTransmit_IT(usartxManger);
		return;
	}
}









/***********************************************************************************************************************/
/*********************************************** USART using DMA *******************************************************/
/***********************************************************************************************************************/
/***********************************************************************************************************************/

// MCALStatus_t HAL_UART_Transmit_DMA(USART_ManagerStruct *huart, const uint8_t *pData, uint16_t Size)
// {
//   const uint32_t *tmp;

//   /* Check that a Tx process is not already ongoing */
//   if (huart->txState == USART_STATE_READY)
//   {
//     if ((pData == NULL) || (Size == 0U))
//     {
//       return MCAL_ERROR;
//     }

//     huart->pTxBuffPtr = pData;
//     huart->TxXferSize = Size;
//     huart->TxXferCount = Size;

//     huart->ErrorCode = USART_ERROR_NONE;
//     huart->txState = USART_STATE_BUSY_TX;

//     /* Set the UART DMA transfer complete callback */
//     huart->hdmatx->XferCpltCallback = UART_DMATransmitCplt;

//     /* Set the UART DMA Half transfer complete callback */
//     huart->hdmatx->XferHalfCpltCallback = UART_DMATxHalfCplt;

//     /* Set the DMA error callback */
//     huart->hdmatx->XferErrorCallback = UART_DMAError;

//     /* Set the DMA abort callback */
//     huart->hdmatx->XferAbortCallback = NULL;

//     /* Enable the UART transmit DMA stream */
//     tmp = (const uint32_t *)&pData;
//     HAL_DMA_Start_IT(huart->hdmatx, *(const uint32_t *)tmp, (uint32_t)&huart->Instance->DR, Size);

//     /* Clear the TC flag in the SR register by writing 0 to it */
//     __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_TC);

//     /* Enable the DMA transfer for transmit request by setting the DMAT bit
//        in the UART CR3 register */
//     ATOMIC_SET_BIT(huart->Instance->CR3, USART_CR3_DMAT);

//     return MCAL_OK;
//   }
//   else
//   {
//     return MCAL_BUSY;
//   }
// }



// MCALStatus_t HAL_UART_Receive_DMA(USART_ManagerStruct *huart, uint8_t *pData, uint16_t Size)
// {
//   /* Check that a Rx process is not already ongoing */
//   if (huart->rxState == USART_STATE_READY)
//   {
//     if ((pData == NULL) || (Size == 0U))
//     {
//       return MCAL_ERROR;
//     }

//     /* Set Reception type to Standard reception */
//     // huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;

//     return (UART_Start_Receive_DMA(huart, pData, Size));
//   }
//   else
//   {
//     return MCAL_BUSY;
//   }
// }

// MCALStatus_t UART_Start_Receive_DMA(USART_ManagerStruct *huart, uint8_t *pData, uint16_t Size)
// {
//   uint32_t *tmp;

//   huart->pRxBuffPtr = pData;
//   huart->RxXferSize = Size;

//   huart->ErrorCode = USART_ERROR_NONE;
//   huart->rxState = USART_STATE_BUSY_RX;

//   /* Set the UART DMA transfer complete callback */
//   huart->hdmarx->XferCpltCallback = UART_DMAReceiveCplt;

//   /* Set the UART DMA Half transfer complete callback */
//   huart->hdmarx->XferHalfCpltCallback = UART_DMARxHalfCplt;

//   /* Set the DMA error callback */
//   huart->hdmarx->XferErrorCallback = UART_DMAError;

//   /* Set the DMA abort callback */
//   huart->hdmarx->XferAbortCallback = NULL;

//   /* Enable the DMA stream */
//   tmp = (uint32_t *)&pData;
//   HAL_DMA_Start_IT(huart->hdmarx, (uint32_t)&huart->Instance->DR, *(uint32_t *)tmp, Size);

//   /* Clear the Overrun flag just before enabling the DMA Rx request: can be mandatory for the second transfer */
//   __HAL_USART_CLEAR_OREFLAG(huart);

//   if (huart->Init.Parity != USART_PARITY_NONE)
//   {
//     /* Enable the UART Parity Error Interrupt */
//     ATOMIC_SET_BIT(huart->Instance->CR1, USART_CR1_PEIE);
//   }

//   /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
//   ATOMIC_SET_BIT(huart->Instance->CR3, USART_CR3_EIE);

//   /* Enable the DMA transfer for the receiver request by setting the DMAR bit
//   in the UART CR3 register */
//   ATOMIC_SET_BIT(huart->Instance->CR3, USART_CR3_DMAR);

//   return MCAL_OK;
// }




