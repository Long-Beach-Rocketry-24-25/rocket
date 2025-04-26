
#pragma once

#include "encoder.h"

#include "stm32l4xx.h"

#include <stdbool.h>

typedef struct
{
    TIM_TypeDef* instance;
} StPrivEncoder;

bool StEncoderInit(Encoder* encoder, StPrivEncoder* st_enc, size_t channel1,
                   size_t channel2);

size_t StEncoderGetCount(Encoder* encoder);

size_t StEncoderGetMaxValue(Encoder* encoder);
