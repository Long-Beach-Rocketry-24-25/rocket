/**
 * 
 */

#pragma once

#include "freertos_timer.h"
#include "gpio_cs.h"
#include "prealloc.h"
#include "st_gpio_fctry.h"
#include "st_i2c_fctry.h"
#include "st_spi_fctry.h"
#include "st_usart_fctry.h"

#include "stm32l4xx_hal.h"
#include "usart_cli.h"

#define DRIVER_MEM_SIZE 0x256

extern TIM_HandleTypeDef htim1;

bool BSP_Init(Usart* debug_usart, I2c* i2c, Spi* spi, Gpio* red_led,
              Gpio* green_led, Gpio* blue_led);

void ConfigIrq(void);

void SystemReset(void);

void USART1_IRQHandler(void);

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
