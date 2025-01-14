

#pragma once

#include "stm32f7xx.h"

#include "gpio.h"
#include "usart.h"
#include "timeout.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct
{
    USART_TypeDef * instance;
    Gpio rx;
    Gpio tx;
    Timeout * timer;
} StPrivUsart;

void StUsartInit(Usart *usart, StPrivUsart *st_usart, uint32_t base_addr, Timeout* timer);

void StUsartConfig(Usart *usart, uint32_t system_core_clk, uint32_t baudrate);

bool StUsartSend(Usart *usart, uint8_t *data, size_t size);

bool StUsartRecv(Usart *usart, uint8_t *data, size_t size);

bool StUsartClearErrors(Usart *usart);

bool StUsartRxReady(Usart* usart);

bool StUsartIsrSet(Usart *usart, uint32_t mask);
