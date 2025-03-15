
#include "uart_pipe.h"

void UartPipeFlush(Usart* u, RingBuffer* rb, uint8_t end)
{
    uint8_t data = 0;

    // Flush all data up to a detected end character.
    while (data != end)
    {
        if (ring_buffer_pop(rb, &data))
        {
            u->send(u, &data, 1);
        }
        else
        {
            break;
        }
    }
}

void UartPipeCallback(Usart* first, Usart* second, RingBuffer* buf, uint8_t end)
{
    uint8_t data = 0;
    if (first->recv(first, &data, 1))
    {
        // Buffer data and flush if needed.
        ring_buffer_insert(buf, data);
        if (data == end)
        {
            UartPipeFlush(second, buf, end);
        }
    }
}
