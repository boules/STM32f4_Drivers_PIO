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
#include "rcc.h"

static uint32_t DMA_CalcBaseAndBitshift(DMA_HandleTypeDef *hdma)
{
  uint32_t stream_number = (((uint32_t)hdma->Instance & 0xFFU) - 16U) / 24U;
  
  /* lookup table for necessary bitshift of flags within status registers */
  static const uint8_t flagBitshiftOffset[8U] = {0U, 6U, 16U, 22U, 0U, 6U, 16U, 22U};
  hdma->StreamIndex = flagBitshiftOffset[stream_number];
  
  if (stream_number > 3U)
  {
    /* return pointer to HISR and HIFCR */
    hdma->StreamBaseAddress = (((uint32_t)hdma->Instance & (uint32_t)(~0x3FFU)) + 4U);
  }
  else
  {
    /* return pointer to LISR and LIFCR */
    hdma->StreamBaseAddress = ((uint32_t)hdma->Instance & (uint32_t)(~0x3FFU));
  }
  
  return hdma->StreamBaseAddress;
}

void DMA_Init(DMA_HandleTypeDef* dmaManager){

	switch (dmaManager->moduleBase)
	{
	case DMA1:
		RCC_Enable(RCC_DMA1);
		break;
	
	case DMA2:
		RCC_Enable(RCC_DMA2);
		break;
	
	default:
		/*ERROR wrong inputs*/
		break;
	}


	/* Change DMA peripheral state */
	hdma->State = HAL_DMA_STATE_BUSY;
	
	//disable the module
	(dmaManager->Instance->CR &=  ~DMA_SxCR_EN);
	/* Check if the DMA Stream is effectively disabled */
	while((dmaManager->Instance->CR & DMA_SxCR_EN) != 0); //maybe you should do time out




	// configure the Control Register
	CLEAR_AND_SET_BYMASKS( dmaManager->Instance->CR
						, 
						(DMA_SxCR_CHSEL | DMA_SxCR_MBURST | DMA_SxCR_PBURST | DMA_SxCR_PL    | DMA_SxCR_MSIZE  | DMA_SxCR_PSIZE  | 
						DMA_SxCR_MINC  | DMA_SxCR_PINC   | DMA_SxCR_CT     | DMA_SxCR_DBM   | DMA_SxCR_DIR   | DMA_SxCR_CIRC)
						,			
						(dmaManager->Init.Channel	| dmaManager->Init.Priority | dmaManager->Init.MemDataAlignment | dmaManager->Init.PeriphDataAlignment | 
						dmaManager->Init.MemInc | dmaManager->Init.PeriphInc | dmaManager->Init.Direction | dmaManager->Init.Mode)
						);
	;


/** not handling fifo mode right now***********************************************************************/
	// if(dmaManager->Init.FIFOMode == DMA_FIFOMODE_ENABLE){
	// 	/* Get memory burst and peripheral burst */
	// 	dmaManager->Instance->CR |=  dmaManager->Init.MemBurst | dmaManager->Init.PeriphBurst;
	// }


	// /* Clear Direct mode and FIFO threshold bits */
	// /* Prepare the DMA Stream FIFO configuration */
	// CLEAR_AND_SET_BYMASKS( dmaManager->Instance->FCR, (DMA_SxFCR_DMDIS | DMA_SxFCR_FTH), (dmaManager->Init.FIFOMode));
	
	// if(dmaManager->Init.FIFOMode == DMA_FIFOMODE_ENABLE){
	// 	/* Get the FIFO threshold */
	// 	dmaManager->Instance->FCR |= dmaManager.FIFOThreshold;
	// }
	// .
	// .
	// .
/***************************************************************************/

// 	/* Clear all interrupt flags */
	/* Initialize StreamBaseAddress and StreamIndex parameters to be used to calculate
     DMA steam Base Address needed by HAL_DMA_IRQHandler() and HAL_DMA_PollForTransfer() */
	DMA_Base_Registers *regs;
  	regs = (DMA_Base_Registers *)DMA_CalcBaseAndBitshift(dmaManager);
  
  	/* Clear all interrupt flags */
  	regs->IFCR = 0x3FU << hdma->StreamIndex;


	/* Initialize the error code */
	dmaManager->ErrorCode = HAL_DMA_ERROR_NONE;

	/* Initialize the DMA state */
	dmaManager->State = HAL_DMA_STATE_READY;

}








static void DMA_SetConfig(DMA_HandleTypeDef* dmaManager, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{
  /* Clear DBM bit */
  dmaManager->Instance->CR &= (uint32_t)(~DMA_SxCR_DBM);

  /* Configure DMA Stream data length */
  dmaManager->Instance->NDTR = DataLength;

  /* Memory to Peripheral */
  if((dmaManager->Init.Direction) == DMA_MEMORY_TO_PERIPH)
  {
    /* Configure DMA Stream destination address */
    dmaManager->Instance->PAR = DstAddress;

    /* Configure DMA Stream source address */
    dmaManager->Instance->M0AR = SrcAddress;
  }
  /* Peripheral to Memory */
  else
  {
    /* Configure DMA Stream source address */
    dmaManager->Instance->PAR = SrcAddress;

    /* Configure DMA Stream destination address */
    dmaManager->Instance->M0AR = DstAddress;
  }
}

// need to put it on .h file
#define IS_DMA_BUFFER_SIZE(SIZE) (((SIZE) >= 0x01U) && ((SIZE) < 0x10000U))
#define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))

void DMA_start(DMA_HandleTypeDef* dmaManager, uint32 SrcAddress, uint32 DstAddress, uint32 DataLength){

	assert_param(IS_DMA_BUFFER_SIZE(DataLength));

	if(HAL_DMA_STATE_READY == dmaManager->State)
	{
		/* Change DMA peripheral state */
		dmaManager->State = HAL_DMA_STATE_BUSY;

		/* Initialize the error code */
		dmaManager->ErrorCode = HAL_DMA_ERROR_NONE;

		/* Configure the source, destination address and the data length */
		DMA_SetConfig(dmaManager, SrcAddress, DstAddress, DataLength);

		/* Enable the Peripheral */
		dmaManager->Instance->CR |=  DMA_SxCR_EN;
	}
}


// howa bytcheck 3la anhi flag  eza kan FULL TRANSFER FLAG or HALF TRANSFER FLAG fi EL ISR REgister  H and low
// TCifx  Transfer complete (interrupt flag)
// HTIFx  half TRansfer flag (interrupt flag)
uint32 DMA_PollForTransfer(HAL_DMA_LevelCompleteTypeDef CompleteLevel){

	uint32 mask_cpltlevel;

	/* Polling mode not supported in circular mode and double buffering mode */
	if ((dmaManager->Instance->CR & DMA_SxCR_CIRC) != 0)   //if circular mode is not rest(is enable)
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






void HAL_DMA_Start_IT(DMA_HandleTypeDef* dmaManager, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{

  /* calculate DMA base and stream number */
  DMA_Base_Registers *regs = (DMA_Base_Registers *)dmaManager->StreamBaseAddress;
  
  /* Check the parameters */
  assert_param(IS_DMA_BUFFER_SIZE(DataLength));
 
  /* Process locked */
//   __HAL_LOCK(hdma);
  
  if(HAL_DMA_STATE_READY == hdma->State)
  {
    /* Change DMA peripheral state */
    dmaManager->State = HAL_DMA_STATE_BUSY;
    
    /* Initialize the error code */
    dmaManager->ErrorCode = HAL_DMA_ERROR_NONE;
    
    /* Configure the source, destination address and the data length */
    DMA_SetConfig(dmaManager, SrcAddress, DstAddress, DataLength);
    
    /* Clear all interrupt flags at correct offset within the register */
    regs->IFCR = 0x3FU << hdma->StreamIndex;
    
    /* Enable Common interrupts*/
    hdma->Instance->CR  |= DMA_IT_TC | DMA_IT_TE | DMA_IT_DME;
    
	// half transfer callback function and interrupt enable
    // if(dmaManager->XferHalfCpltCallback != NULL)
    // {
    //   dmaManager->Instance->CR  |= DMA_IT_HT;
    // }
    
    /* Enable the Peripheral */
		dmaManager->Instance->CR |=  DMA_SxCR_EN;
//   }
//   else
//   {
//     /* Process unlocked */
//     __HAL_UNLOCK(hdma);	  
    
//     /* Return error status */
//     status = HAL_BUSY;
//   }
  
  return;
}