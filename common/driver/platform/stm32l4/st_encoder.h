
#pragma once

#include "encoder.h"

#include "stm32l4xx.h"

#include <stdbool.h>
#include <stdint.h>

#define ST_ENCODER_CHANNEL_PAIR_MAX 2

/**
 * Configures polarity of capture/compare input to TI number.
 * ONE_TO_ONE maps IC1 to TI1 and IC2 to TI2, and SWAPPED maps
 * IC1 to TI2 and IC2 to TI1.
 */
typedef enum
{
    ONE_TO_ONE = 0b01,
    SWAPPED = 0b10
} StEncoderCmpChannel;

typedef struct
{
    TIM_TypeDef* instance;
} StPrivEncoder;

bool StEncoderInit(Encoder* encoder, StPrivEncoder* st_enc, uint32_t base_addr,
                   size_t channel_pair, StEncoderCmpChannel channel_polarity);

size_t StEncoderGetCount(Encoder* encoder);

size_t StEncoderGetMaxValue(Encoder* encoder);

size_t StEncoderCountsPerCycle(Encoder* encoder);
