
#pragma once

#include "alloc.h"
#include "exit.h"
#include "st_gpio_fctry.h"
#include "st_usart.h"

bool MakeStUsart(Usart* usart, Mem* mem, uint32_t base_addr,
                 uint32_t sys_core_clk, uint32_t baudrate, Timeout* timer,
                 StGpioParams io1, StGpioParams io2);