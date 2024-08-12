

#pragma once

#include "bus.h"
#include "st_usart_def.h"
#include "timeout.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Universal Synchronous Asynchronous Receiver Transmitter
 */
typedef struct
{
    volatile uint32_t CR1;         /*!< USART Control register 1,                 Address offset: 0x00 */
    volatile uint32_t CR2;         /*!< USART Control register 2,                 Address offset: 0x04 */
    volatile uint32_t CR3;         /*!< USART Control register 3,                 Address offset: 0x08 */
    volatile uint32_t BRR;         /*!< USART Baud rate register,                 Address offset: 0x0C */
    volatile uint16_t GTPR;        /*!< USART Guard time and prescaler register,  Address offset: 0x10 */
    uint16_t  RESERVED2;       /*!< Reserved, 0x12                                                 */
    volatile uint32_t RTOR;        /*!< USART Receiver Time Out register,         Address offset: 0x14 */
    volatile uint16_t RQR;         /*!< USART Request register,                   Address offset: 0x18 */
    uint16_t  RESERVED3;       /*!< Reserved, 0x1A                                                 */
    volatile uint32_t ISR;         /*!< USART Interrupt and status register,      Address offset: 0x1C */
    volatile uint32_t ICR;         /*!< USART Interrupt flag Clear register,      Address offset: 0x20 */
    volatile uint16_t RDR;         /*!< USART Receive Data register,              Address offset: 0x24 */
    uint16_t  RESERVED4;       /*!< Reserved, 0x26                                                 */
    volatile uint16_t TDR;         /*!< USART Transmit Data register,             Address offset: 0x28 */
    uint16_t  RESERVED5;       /*!< Reserved, 0x2A                                                 */
} ST_USART_TypeDef;

typedef struct
{
    ST_USART_TypeDef * bus;
    Timeout * timer;
} Usart;

void Usart_Init(Usart *usart, uint32_t base_addr, Timeout* timer);

void Usart_Config(Usart *usart, uint32_t system_core_clk, uint32_t baudrate);

bool Usart_Send(Usart *usart, uint8_t *data, size_t size);

bool Usart_Recv(Usart *usart, uint8_t *data, size_t size);

bool Usart_Isr_Set(Usart *usart, uint32_t mask);
