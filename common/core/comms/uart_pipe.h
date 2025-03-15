/**
 * A UART pipe that channels input/output from one UART to
 * input/output of another, as well as the other way around.
 * Assumes the UART is implemented using interrupts, and
 * provides two callbacks accordingly.
 * 
 * This is a fairly simple, non-robust implementation considering
 * the use of interrupts. There will be some data loss if bytes are
 * received while buffers are being flushed. This is fine for most
 * basic purposes, which is what this is written for.
 */

#pragma once

#include "ring_buffer.h"
#include "usart.h"

/**
 * Flushes the given buffer with the given UART.
 * 
 * @param u the UART to flush through.
 * @param rb the ring buffer to flush.
 * @param end the end character to flush up to.
 */
void UartPipeFlush(Usart* u, RingBuffer* rb, uint8_t end);

/**
 * Interrupt callback for piping first UART to second UART.
 * 
 * @param first the UART to receive data from.
 * @param second the UART to flush through.
 * @param rb the ring buffer to flush.
 * @param end the character to flush ring buffers on.
 */
void UartPipeCallback(Usart* first, Usart* second, RingBuffer* buf,
                      uint8_t end);