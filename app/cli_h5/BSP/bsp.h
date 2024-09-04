/**
 * 
 */

#pragma once

#include "stm32h503xx.h"
#include "stm32h5xx_hal.h"
#include "st_usart.h"
#include "st_i2c.h"
#include "retry_timer.h"

void BSP_Init(Usart *usart, I2c *i2c);

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