

#pragma once

#include "freertos_timer.h"
#include "st_gpio.h"
#include "st_pwm.h"
#include "st_usart.h"
#include "stm32f746xx.h"
#include "usart_cli.h"

void BSP_Init(Usart* usart, Pwm* pwm_timer, Gpio* led_gpio);

void USART3_IRQHandler(void);