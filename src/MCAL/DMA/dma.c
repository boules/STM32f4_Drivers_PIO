// {
// 	__HAL_RCC_DMA2_CLK_ENABLE();

//   /* DMA interrupt init */
//   /* DMA2_Stream2_IRQn interrupt configuration */
//   HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
//   HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
// }


//Microcontroller Specific Part

#include "dma.h"
#include "dma_regs.h"
#include "common_macros.h"
#include "platform_types.h"


void DMA_Init(DMA_InitTypeDef* configPtr){
	//disable the module
	(DMA2_Stream2->CR &=  ~DMA_SxCR_EN);

	/* Check if the DMA Stream is effectively disabled */
	while((DMA2_Stream2->CR & DMA_SxCR_EN) != 0);

	// configure the Control Register
	CLEAR_AND_SET_BYMASKS( DMA2_Stream2->CR
						, 
						(DMA_SxCR_CHSEL | DMA_SxCR_MBURST | DMA_SxCR_PBURST | DMA_SxCR_PL    | DMA_SxCR_MSIZE  | DMA_SxCR_PSIZE  | 
						DMA_SxCR_MINC  | DMA_SxCR_PINC   | DMA_SxCR_CIRC   | DMA_SxCR_DIR   | DMA_SxCR_CT     | DMA_SxCR_DBM)
						,			
						(configPtr->Channel	| configPtr->Direction | configPtr->PeriphInc | configPtr->MemInc |
						configPtr->PeriphDataAlignment | configPtr->MemDataAlignment | configPtr->Mode | configPtr->Priority)
						);
	;

	if(configPtr->FIFOMode == DMA_FIFOMODE_ENABLE){
		/* Get memory burst and peripheral burst */
		DMA1_Stream2->CR |=  configPtr->MemBurst | configPtr->PeriphBurst;
	}

	/********************not handling FIFO***********/
	CLEAR_AND_SET_BYMASKS( DMA2_Stream2->FCR, (DMA_SxFCR_DMDIS | DMA_SxFCR_FTH), (configPtr->FIFOMode));
	
	if(configPtr->FIFOMode == DMA_FIFOMODE_ENABLE){
		/* Get the FIFO threshold */
		DMA2_Stream2->FCR |= configPtr->FIFOThreshold;
	}
	/************************************************/

// not REALY handling any interrupts
// 	/* Clear all interrupt flags */
//   regs->IFCR = 0x3FU << hdma->StreamIndex;


}








static void DMA_SetConfig(uint32 stream, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{
  /* Clear DBM bit */
  DMA2_Stream2->CR &= (uint32_t)(~DMA_SxCR_DBM);

  /* Configure DMA Stream data length */
  DMA2_Stream2->NDTR = DataLength;

//   /* Memory to Peripheral */
//   if((hdma->Init.Direction) == DMA_MEMORY_TO_PERIPH)
//   {
//     /* Configure DMA Stream destination address */
//     DMA2_Stream2->PAR = DstAddress;

//     /* Configure DMA Stream source address */
//     DMA2_Stream2->M0AR = SrcAddress;
//   }
//   /* Peripheral to Memory */
//   else
  {
    /* Configure DMA Stream source address */
    DMA2_Stream2->PAR = SrcAddress;

    /* Configure DMA Stream destination address */
    DMA2_Stream2->M0AR = DstAddress;
  }
}


void DMA_start(uint32 stream, uint32 SrcAddress, uint32 DstAddress, uint32 DataLength){

	/* Configure the source, destination address and the data length */
    DMA_SetConfig(DMA2_Stream2, SrcAddress, DstAddress, DataLength);

    /* Enable the Peripheral */
	DMA2_Stream2->CR |=  DMA_SxCR_EN;
}


// howa bytcheck 3la anhi flag  eza kan FULL TRANSFER FLAG or HALF TRANSFER FLAG fi EL ISR REgister  H and low
// TCifx  Transfer complete (interrupt flag)
// HTIFx  half TRansfer flag (interrupt flag)
uint32 DMA_PollForTransfer(HAL_DMA_LevelCompleteTypeDef CompleteLevel){

	uint32 mask_cpltlevel;

	/* Polling mode not supported in circular mode and double buffering mode */
	if ((DMA2_Stream2->CR & DMA_SxCR_CIRC) != 0)   //if circular mode is not rest(is enable)
	{
		//error
	}



	/* Get the level transfer complete flag */
  if(CompleteLevel == HAL_DMA_FULL_TRANSFER)
  {
    /* Transfer Complete flag */
    mask_cpltlevel = DMA_FLAG_TCIF0_4 << 16; // 16 is only for stream2
  }
  else
  {
    /* Half Transfer Complete flag */
    mask_cpltlevel = DMA_FLAG_HTIF0_4 << 16; // 16 is only for stream2
  }


	//poll Wait till the flag is raised
	while((DMA2->LISR & mask_cpltlevel) == 0);






	if(CompleteLevel == HAL_DMA_FULL_TRANSFER)
	{
	/* Clear the half transfer and transfer complete flags */
	DMA2->LIFCR = (DMA_FLAG_HTIF0_4 | DMA_FLAG_TCIF0_4) << 16;   //hdma->StreamIndex = 16


	}
	else
	{
	/* Clear the half transfer and transfer complete flags */
	DMA2->LIFCR = (DMA_FLAG_HTIF0_4) << 16;   //hdma->StreamIndex = 16
	}

}



// //DMA_HandleTypeDef *hdma
// // instance is the base address of the stream reg stuct
// static uint32_t DMA_CalcBaseAndBitshift(DMA_Stream_TypeDef* streamAddressBaseNumber)
// {
// 	//(0x10 & 0x018) are magic number that conclude the number of the stream from the address
//   uint32_t stream_number = (((uint32_t)streamAddressBaseNumber & 0xFFU) - 0x10U) / 0x18U;
  
//   /* lookup table for necessary bitshift of flags within status registers */
//   static const uint8_t flagBitshiftOffset[8U] = {0U, 6U, 16U, 22U, 0U, 6U, 16U, 22U};
//   hdma->StreamIndex = flagBitshiftOffset[stream_number]; /* *****you need to make a handler STRUCT ******* */
  
//   if (stream_number > 3U)
//   {
//     /* return pointer to HISR and HIFCR */
//     hdma->StreamBaseAddress = (((uint32_t)hdma->Instance & (uint32_t)(~0x3FFU)) + 4U);
//   }
//   else
//   {
//     /* return pointer to LISR and LIFCR */
//     hdma->StreamBaseAddress = ((uint32_t)hdma->Instance & (uint32_t)(~0x3FFU));
//   }
  
//   return hdma->StreamBaseAddress;
// }