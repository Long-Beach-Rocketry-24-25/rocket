
#include "uart_pipe.h"

static Usart* u1;
static Usart* u2;
static RingBuffer* rb1;
static RingBuffer* rb2;
static uint8_t end;

void UartPipeInit(Usart* usart1, Usart* usart2, RingBuffer* buf1,
                  RingBuffer* buf2, uint8_t end_char)
{
    u1 = usart1;
    u2 = usart2;
    rb1 = buf1;
    rb2 = buf2;
    end = end_char;
}

void UartPipeFlush(Usart* u, RingBuffer* rb)
{
    uint8_t data = 0;
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

void UartPipeCallback1(void)
{
    uint8_t data = 0;
    if (u1->recv(u1, &data, 1))
    {
        ring_buffer_insert(rb1, data);
        if (data == end)
        {
            UartPipeFlush(u2, rb1);
        }
    }
}

void UartPipeCallback2(void)
{
    uint8_t data = 0;
    if (u2->recv(u2, &data, 1))
    {
        ring_buffer_insert(rb2, data);
        if (data == end)
        {
            UartPipeFlush(u1, rb2);
        }
    }
}
