/**
 * 
 */

#pragma once

#include "stm32h503xx.h"
#include "stm32h5xx_hal.h"

void BSP_Init(void);

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void);

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void);