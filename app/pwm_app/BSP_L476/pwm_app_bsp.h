/**
 * 
 */

#pragma once

#include "freertos_timer.h"
#include "st_gpio.h"
#include "st_pwm.h"
#include "st_usart.h"
#include "stm32l4xx_hal.h"
#include "usart_cli.h"

extern TIM_HandleTypeDef htim1;

void BSP_Init(Usart* usart, Pwm* pwm, Gpio* led_gpio);

void USART2_IRQHandler(void);

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
