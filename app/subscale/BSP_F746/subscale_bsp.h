/**
 * 
 */

#pragma once

#include "stm32f7xx_hal.h"
#include "st_usart.h"
#include "st_spi.h"
#include "st_i2c.h"
#include "st_gpio.h"
#include "gpio_cs.h"
#include "freertos_timer.h"
#include "usart_cli.h"


void BSP_Init(Usart *usart, Spi *spi, I2c *i2c, Gpio *led_gpio);

void USART3_IRQHandler(void);
