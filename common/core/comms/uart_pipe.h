
#pragma once

#include "ring_buffer.h"
#include "usart.h"

void UartPipeInit(Usart* usart1, Usart* usart2, RingBuffer* buf1,
                  RingBuffer* buf2, uint8_t end_char);
void UartPipeFlush(Usart* u, RingBuffer* rb);
void UartPipeCallback1(void);
void UartPipeCallback2(void);
