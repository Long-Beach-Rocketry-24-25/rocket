

#pragma once

#include "freertos_timer.h"
#include "nidec_24h.h"
#include "prealloc.h"
#include "st_encoder_fctry.h"
#include "st_gpio_fctry.h"
#include "st_pwm_fctry.h"
#include "st_usart_fctry.h"
#include "stm32l412xx.h"
#include "usart_cli.h"

#define DRIVER_MEM_SIZE 0x256

bool BSP_Init(Usart* usart, Motor* motor, Encoder* encoder);

void USART2_IRQHandler(void);