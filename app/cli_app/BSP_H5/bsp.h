/**
 * 
 */

#pragma once

#include "stm32h5xx.h"
#include "st_usart.h"
#include "st_i2c.h"
#include "st_gpio.h"
#include "freertos_timer.h"

#define USART_HANDLER_FUNC void USART3_IRQHandler(void)

#define ADDITIONAL_HANDLER_FUNCS

void BSP_Init(Usart *usart, I2c *i2c, Gpio *led_gpio);

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