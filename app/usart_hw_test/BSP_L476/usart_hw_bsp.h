/**
 * 
 */

#pragma once

#include "freertos_timer.h"
#include "snx5176b.h"
#include "st_gpio.h"
#include "st_i2c.h"
#include "st_usart.h"
#include "stm32l4xx_hal.h"
#include "uart_pipe.h"

#define UART_PIPE_BUF_SIZE 256
#define UART_END_CHAR '\n'

extern TIM_HandleTypeDef htim1;

void BSP_Init(Usart* cli_usart, Usart* comm_usart, Gpio* led_gpio);

void USART2_IRQHandler(void);

void USART3_IRQHandler(void);

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
