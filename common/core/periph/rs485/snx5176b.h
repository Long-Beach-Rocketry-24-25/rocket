
#pragma once

#include "gpio.h"
#include "ring_buffer.h"
#include "usart.h"

typedef struct
{
    Usart usart;
    Gpio txe;
    Gpio rxe;
    RingBuffer buf;
} Snx5176b;

void Snx5176bInit(Usart* rs485, Snx5176b* chip);
void Snx5176bConfig(Usart* rs485);
bool Snx5176bSend(Usart* rs485, const uint8_t* data, size_t size);
bool Snx5176bRecv(Usart* rs485, uint8_t* data, size_t size);
void Snx5176bTxSetEn(Snx5176b* chip, bool enable);
