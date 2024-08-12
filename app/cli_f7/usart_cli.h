
#pragma once

#include "FreeRTOS.h"
#include "task.h"

#include "ring_buffer.h"
#include "st_usart.h"
#include "retry_timer.h"

#include "cli.h"

void usart_rx_callback();

bool create_cli_task(uint32_t usart_base_addr, uint32_t sys_core_clk,
                     Command * commands, size_t num_commands);