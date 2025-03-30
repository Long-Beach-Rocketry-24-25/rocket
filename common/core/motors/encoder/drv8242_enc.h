#pragma once

#include "gpio.h"
#include "pwm.h"
#include "qenc.h"

typedef struct
{
    QEnc* instance;
    size_t ticks_needed;
    size_t angle;
    size_t tick_size;
    uint16_t counter;
    size_t ticks_per_angle;
} DCEncParams;

void QEnc_Init(QEnc* qenc, DCEncParams* params);
bool get_ticks(QEnc* qenc, size_t angle, size_t tick_size, size_t ppr);
bool set_ticks(QEnc* qenc);
