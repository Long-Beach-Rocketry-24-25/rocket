

#pragma once


#include "st_pwm.h"
#include "stm32l4xx_hal_tim.h"
#include "st_gpio.h"

void BSP_Init(Pwm* pwm_timer, Gpio* led_gpio, size_t base_address, size_t mc_clock, bool enable, size_t hz, size_t duty);

