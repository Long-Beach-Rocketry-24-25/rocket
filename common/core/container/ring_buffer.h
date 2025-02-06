/**
 * 
 * 
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct
{
    uint8_t* data;
    size_t head;
    size_t tail;
    size_t max_len;
    size_t curr_size;
} RingBuffer;

void ring_buffer_init(RingBuffer* rb, uint8_t* data, size_t size);

bool ring_buffer_insert(RingBuffer* rb, uint8_t data);

bool ring_buffer_pop(RingBuffer* rb, uint8_t* data);