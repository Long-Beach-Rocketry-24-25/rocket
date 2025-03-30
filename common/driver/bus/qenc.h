#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "stm32l4xx.h"

typedef struct QEnc QEnc;

struct QEnc
{
    bool (*getTicks)(QEnc* qenc, size_t angle, size_t tick_size);

    void* priv;
};