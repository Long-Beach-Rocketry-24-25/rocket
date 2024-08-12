
#include "st_usart.h"


void Usart_Init(Usart *usart, uint32_t base_addr, Timeout* timer)
{
    usart->bus = (ST_USART_TypeDef *) base_addr;
    usart->timer = timer;
}


void Usart_Config(Usart *usart, uint32_t system_core_clk, uint32_t baudrate)
{
    uint16_t uartdiv = system_core_clk / baudrate;

    usart->bus->BRR = (((uartdiv / 16) << USART_BRR_DIV_MANTISSA_Pos)
                | ((uartdiv % 16) << USART_BRR_DIV_FRACTION_Pos));
    usart->bus->CR1 |= USART_CR1_UE
                | USART_CR1_RE
                | USART_CR1_TE;
    usart->bus->CR1 |= USART_CR1_RXNEIE;
}

bool Usart_Send(Usart *usart, uint8_t *data, size_t size)
{
    for (size_t i = 0; i < size; ++i) {

        WAIT(usart->timer, usart->bus->ISR & USART_ISR_TXE, false);
        usart->bus->TDR = data[i];

    }

    WAIT(usart->timer, usart->bus->ISR & USART_ISR_TC, false);

    return true;
}

bool Usart_Recv(Usart *usart, uint8_t *data, size_t size)
{
    size_t count = 0;
    while (count < size)
    {
        WAIT(usart->timer, usart->bus->ISR & USART_ISR_RXNE, false);
        data[count++] = usart->bus->RDR;
    }

    return true;
}