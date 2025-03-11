
#include "st_usart_fctry.h"

bool MakeStUsart(Usart* usart, Mem* mem, uint32_t base_addr,
                 uint32_t sys_core_clk, uint32_t baudrate, Timeout* timer,
                 StGpioParams io1, StGpioParams io2)
{
    StPrivUsart* st = ALLOC(mem, StPrivUsart);
    EXIT_IF(st == NULL, false);

    st->instance = (USART_TypeDef*)base_addr;
    st->timer = timer;
    EXIT_IF(!MakeStGpio(&st->rx, mem, io1), false);
    EXIT_IF(!MakeStGpio(&st->tx, mem, io2), false);

    usart->priv = (void*)st;
    usart->send = StUsartSend;
    usart->recv = StUsartRecv;

    StUsartConfig(usart, sys_core_clk, baudrate);

    return true;
}