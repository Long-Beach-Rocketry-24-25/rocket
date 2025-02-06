/**
 * 
 * 
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct Usart Usart;

struct Usart
{
    bool (*send)(Usart* usart, uint8_t* data, size_t size);
    bool (*recv)(Usart* usart, uint8_t* data, size_t size);

    void* priv;
};