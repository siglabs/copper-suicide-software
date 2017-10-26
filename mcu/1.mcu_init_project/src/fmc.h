/*
 * fmc.h
 *
 *  Created on: Jun 9, 2017
 *      Author: sai
 */
//  ******************************************************************************
//  * File Name          : FMC.h
//  * Description        : This file provides code for the configuration
//  *                      of the FMC peripheral.
//  ******************************************************************************


#ifndef FMC_H_
#define FMC_H_
#ifdef __cplusplus
 extern "C" {
#endif

 /* Includes ------------------------------------------------------------------*/
 #include "stm32f7xx_hal.h"

 /* USER CODE BEGIN Includes */

 /* USER CODE END Includes */

 extern NOR_HandleTypeDef hnor1;
 extern void Error_Handler(void);

 /* USER CODE BEGIN Private defines */

 /* USER CODE END Private defines */

 void MX_FMC_Init(void);
 void HAL_NOR_MspInit(NOR_HandleTypeDef* hnor);
 void HAL_NOR_MspDeInit(NOR_HandleTypeDef* hnor);

 /* USER CODE BEGIN Prototypes */

 /* USER CODE END Prototypes */

 #ifdef __cplusplus
 }
 #endif


#endif /* FMC_H_ */
