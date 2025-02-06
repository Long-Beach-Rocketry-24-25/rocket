#include "ring_buffer.h"

void ring_buffer_init(RingBuffer* rb, uint8_t* data, size_t size)
{
    rb->data = data;
    rb->max_len = size;
    rb->curr_size = 0;
    rb->head = 0;
    rb->tail = 0;
}

bool ring_buffer_insert(RingBuffer* rb, uint8_t data)
{
    size_t next = (rb->head + 1) % rb->max_len;
    if (rb->curr_size == rb->max_len)
    {
        return false;
    }
    rb->data[rb->head] = data;
    rb->head = next;
    rb->curr_size++;
    return true;
}

bool ring_buffer_pop(RingBuffer* rb, uint8_t* data)
{
    if (rb->curr_size == 0)
    {
        return false;
    }
    *data = rb->data[rb->tail];
    rb->tail = (rb->tail + 1) % rb->max_len;
    rb->curr_size--;
    return true;
}