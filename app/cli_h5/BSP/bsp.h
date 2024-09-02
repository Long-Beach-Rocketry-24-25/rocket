/**
 * 
 */

#pragma once

#include "stm32h503xx.h"
// #include "stm32h5xx_hal.h"
#include "st_usart.h"
#include "retry_timer.h"

void BSP_Init(Usart *usart);
