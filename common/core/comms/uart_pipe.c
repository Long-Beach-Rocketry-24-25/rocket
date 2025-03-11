
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

static inline void callback(Usart* first, Usart* second, RingBuffer* buf)
{
    uint8_t data = 0;
    if (first->recv(first, &data, 1))
    {
        // Buffer data and flush if needed.
        ring_buffer_insert(buf, data);
        if (data == end)
        {
            UartPipeFlush(second, buf);
        }
    }
}

void UartPipeCallback1(void)
{
    callback(u1, u2, rb1);
}

void UartPipeCallback2(void)
{
    callback(u2, u1, rb2);
}
