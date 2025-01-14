
#include "st_usart.h"


void StUsartInit(Usart *usart, StPrivUsart *st_usart, uint32_t base_addr, Timeout* timer)
{
    st_usart->instance = (USART_TypeDef *) base_addr;
    st_usart->timer = timer;

    usart->priv = (void *) st_usart;
    usart->send = StUsartSend;
    usart->recv = StUsartRecv;
    usart->clear_errors = StUsartClearErrors;
    usart->rx_ready = StUsartRxReady;
}


void StUsartConfig(Usart *usart, uint32_t system_core_clk, uint32_t baudrate)
{
    StPrivUsart * dev = (StPrivUsart*) usart->priv;

    /*
     * Setup gpio.
     */
    dev->rx.config(&dev->rx);
    dev->tx.config(&dev->tx);

    uint16_t uartdiv = system_core_clk / baudrate;

    dev->instance->BRR = (((uartdiv / 16) << 4)
                | ((uartdiv % 16) << 0));
    dev->instance->CR1 |= USART_CR1_UE
                | USART_CR1_RE
                | USART_CR1_TE;
    dev->instance->CR1 |= USART_CR1_RXNEIE;
}

bool StUsartSend(Usart *usart, uint8_t *data, size_t size)
{
    StPrivUsart * dev = (StPrivUsart*) usart->priv;
    for (size_t i = 0; i < size; ++i) {

        WAIT(dev->timer, dev->instance->ISR & USART_ISR_TXE, false);
        dev->instance->TDR = data[i];

    }

    WAIT(dev->timer, dev->instance->ISR & USART_ISR_TC, false);

    return true;
}

bool StUsartRecv(Usart *usart, uint8_t *data, size_t size)
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

bool StUsartClearErrors(Usart *usart)
{
    StPrivUsart * dev = (StPrivUsart*) usart->priv;
    if (dev->instance->ISR & USART_ISR_ORE)
    {
        dev->instance->ICR |= USART_ICR_ORECF;
    }
    
    return true;
}

bool StUsartIsrSet(Usart *usart, uint32_t mask)
{
    StPrivUsart * dev = (StPrivUsart*) usart->priv;
    return dev->instance->ISR & mask;
}

bool StUsartRxReady(Usart *usart)
{
    return StUsartIsrSet(usart, USART_ISR_RXNE);
}