/**
 * A UART pipe that channels input/output from one UART to
 * input/output of another, as well as the other way around.
 * Assumes the UART is implemented using interrupts, and
 * provides two callbacks accordingly.
 */

#pragma once

#include "ring_buffer.h"
#include "usart.h"

/**
 * Initializes the UART pipe.
 * 
 * This is a fairly simple, non-robust implementation considering
 * the use of interrupts. There will be some data loss if bytes are
 * received while buffers are being flushed. This is fine for most
 * basic purposes, which is what this is written for.
 * 
 * @param usart1 the first UART.
 * @param usart2 the second UART.
 * @param buf1 ring buffer for storing data received by first UART.
 * @param buf2 ring buffer for storing data received by second UART.
 * @param end_char the character to flush ring buffers on.
 */
void UartPipeInit(Usart* usart1, Usart* usart2, RingBuffer* buf1,
                  RingBuffer* buf2, uint8_t end_char);

/**
 * Flushes the given buffer with the given UART.
 * 
 * @param u the UART to flush through.
 * @param rb the ring buffer to flush.
 */
void UartPipeFlush(Usart* u, RingBuffer* rb);

/**
 * Callback for the first UART provided during initialization.
 * Needs to be placed in the IRQ Handler corresponding to that UART.
 */
void UartPipeCallback1(void);

/**
 * Callback for the second UART provided during initialization.
 * Needs to be placed in the IRQ Handler corresponding to that UART.
 */
void UartPipeCallback2(void);
