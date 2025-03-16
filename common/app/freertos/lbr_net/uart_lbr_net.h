#pragma once

#include "lbr_net.h"
#include "net_interface.h"

#include "retry_timer.h"
#include "ring_buffer.h"
#include "usart.h"

void usart_rx_callback();

void uart_lbr_init(uint8_t address, net_if net_if);