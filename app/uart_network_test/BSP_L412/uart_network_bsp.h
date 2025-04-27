

#pragma once

#include "freertos_timer.h"
#include "prealloc.h"
#include "st_usart_fctry.h"
#include "stm32l412xx.h"
#include "usart_cli.h"

#define DRIVER_MEM_SIZE 0x256

bool BSP_Init(Usart* usart);

void USART2_IRQHandler(void);