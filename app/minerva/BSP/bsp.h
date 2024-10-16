/**
 * 
 */

#pragma once

#include "stm32l4xx.h"
#include "stm32l452xx.h"
#include "st_usart.h"
#include "st_i2c.h"
#include "st_bxcan.h"
#include "st_gpio.h"
#include "freertos_timer.h"

void BSP_Init(Usart *usart, I2c *temp_i2c, I2c *an1_i2c,
			  I2c *an2_i2c, CanBus * can, Gpio *led_gpio);

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