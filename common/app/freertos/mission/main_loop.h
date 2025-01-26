

#pragma once

#include "FreeRTOS.h"
#include "task.h"

#define MAIN_LOOP_STACK_SIZE_BYTES 0x1000

void create_main_loop(void (*func) (void), size_t frequency_hz);
