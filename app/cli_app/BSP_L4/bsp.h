/**
 * 
 */

#pragma once

#include "stm32l476xx.h"
#include "stm32l4xx_hal.h"
#include "st_usart.h"
#include "st_i2c.h"
#include "st_gpio.h"
#include "freertos_timer.h"
#include "usart_cli.h"

extern TIM_HandleTypeDef htim1;

void BSP_Init(Usart *usart, I2c *i2c, Gpio *led_gpio);

void USART2_IRQHandler(void);

void TIM1_UP_TIM16_IRQHandler(void);

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
