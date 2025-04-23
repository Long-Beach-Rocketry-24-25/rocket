

#pragma once

#include "freertos_timer.h"
#include "st_gpio.h"
#include "st_pwm.h"
#include "st_usart.h"
#include "stm32l412xx.h"
#include "usart_cli.h"

#define DRIVER_MEM_SIZE 0x256

void BSP_Init(Usart* usart, Pwm* pwm, Gpio* led_gpio);

void USART2_IRQHandler(void);