
#include "st_usart.h"


void St_Usart_Init(Usart *usart, StPrivUsart *st_usart, uint32_t base_addr, Timeout* timer)
{
    st_usart->instance = (ST_USART_TypeDef *) base_addr;
    st_usart->timer = timer;

    usart->priv = (void *) st_usart;
    usart->send = St_Usart_Send;
    usart->recv = St_Usart_Recv;
    usart->clear_errors = St_Usart_Clear_Errors;
    usart->rx_ready = St_Usart_Rx_Ready;
}


void St_Usart_Config(Usart *usart, uint32_t system_core_clk, uint32_t baudrate)
{
    StPrivUsart * dev = (StPrivUsart*) usart->priv;
    uint16_t uartdiv = system_core_clk / baudrate;

    dev->instance->BRR = (((uartdiv / 16) << USART_BRR_DIV_MANTISSA_Pos)
                | ((uartdiv % 16) << USART_BRR_DIV_FRACTION_Pos));
    dev->instance->CR1 |= USART_CR1_UE
                | USART_CR1_RE
                | USART_CR1_TE;
    dev->instance->CR1 |= USART_CR1_RXNEIE;
}

bool St_Usart_Send(Usart *usart, uint8_t *data, size_t size)
{
    StPrivUsart * dev = (StPrivUsart*) usart->priv;
    for (size_t i = 0; i < size; ++i) {

        WAIT(dev->timer, dev->instance->ISR & USART_ISR_TXE, false);
        dev->instance->TDR = data[i];

    }

    WAIT(dev->timer, dev->instance->ISR & USART_ISR_TC, false);

    return true;
}

bool St_Usart_Recv(Usart *usart, uint8_t *data, size_t size)
{
    StPrivUsart * dev = (StPrivUsart*) usart->priv;
    size_t count = 0;
    while (count < size)
    {
        WAIT(dev->timer, dev->instance->ISR & USART_ISR_RXNE, false);
        data[count++] = dev->instance->RDR;
    }

    return true;
}

bool St_Usart_Clear_Errors(Usart *usart)
{
    StPrivUsart * dev = (StPrivUsart*) usart->priv;
    if (dev->instance->ISR & USART_ISR_ORE)
    {
        dev->instance->ICR |= USART_ICR_ORECF;
    }
}

bool St_Usart_Isr_Set(Usart *usart, uint32_t mask)
{
    StPrivUsart * dev = (StPrivUsart*) usart->priv;
    return dev->instance->ISR & mask;
}

bool St_Usart_Rx_Ready(Usart *usart)
{
    return St_Usart_Isr_Set(usart, USART_ISR_RXNE);
}