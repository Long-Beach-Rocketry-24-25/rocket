/**
 * Usart example CLI.
 * 
 * Author: Nick Fan
 */

#pragma once

#include "FreeRTOS.h"
#include "task.h"

#include "ring_buffer.h"
#include "st_usart.h"
#include "retry_timer.h"

#include "cli.h"

#define CLI_VER "usart-cli-v0.1"
#define PASSWORD "LBR"
#define MAX_SEND_LEN 60

/**
 * Usart RXNE callback, to be called in 
 * USARTx_IRQHandler().
 */
void usart_rx_callback();

/**
 * Writes a null terminated string to the command line, automatically
 * adding a newline. 
 * 
 * @returns True if successful, false otherwise.
 */
bool cli_write(const char* data);

/**
 * Creates a FreeRTOS CLI processing task using a given USART/core
 * clock frequency. A list of commands can be registered to add
 * functionality to the command line.
 * 
 * @param usart_base_addr 32-bit peripheral base address.
 * @param sys_core_clk the system core clock frequency.
 * @param commands a list of commands to register.
 * @param num_commands the size of the command list.
 * 
 * @returns True if all commands are registered and the task is created,
 *          otherwise false.
 */
bool create_cli_task(uint32_t usart_base_addr, uint32_t sys_core_clk,
                     Command * commands, size_t num_commands);