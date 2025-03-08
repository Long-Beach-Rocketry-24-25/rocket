/**
 * 
 */

#pragma once

#include "freertos_timer.h"
#include "st_gpio.h"
#include "st_usart.h"
#include "stm32f746xx.h"
#include "stm32f7xx_hal.h"
#include "uart_pipe.h"

#define UART_PIPE_BUF_SIZE 256

void BSP_Init(Usart* cli_usart, Usart* comm_usart, Gpio* led_gpio);

void USART3_IRQHandler(void);

void UART4_IRQHandler(void);
