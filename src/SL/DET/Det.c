 /******************************************************************************
 *
 * Module: Det
 *
 * File Name: Det.c
 *
 * Description:  Det stores the development errors reported by other modules.
 *
 * Author: Boules
 ******************************************************************************/

#include "Det.h"
#include "common_macros.h"

Std_ReturnType Det_ReportError( uint16 ModuleId,
								uint8 InstanceId,
								uint8 ApiId,
								uint8 ErrorId )
{	
	Disable_Interrupts();
    while(1)
    {

    }
    return E_OK;
}



// /**
//   * @brief  This function is executed in case of error occurrence.
//   * @retval None
//   */
// void Error_Handler(void)
// {
//   /* USER CODE BEGIN Error_Handler_Debug */
//   /* User can add his own implementation to report the HAL error return state */
//   __disable_irq();
//   while (1)
//   {
//   }
//   /* USER CODE END Error_Handler_Debug */
// }
