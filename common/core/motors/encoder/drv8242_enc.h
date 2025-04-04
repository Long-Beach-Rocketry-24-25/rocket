#pragma once

#include "dcm.h"
#include "gpio.h"
#include "pwm.h"
#include "qenc.h"

typedef enum
{
    idle = 0,
    rotating,

} QEncState;

typedef struct
{
    QEnc* encoder;
    DCMotor* motor;
    QEncState state;
    size_t ticks_needed;
    size_t ppr;
    size_t last_enc;
    uint16_t counter;
    size_t diff;
    size_t ticks_per_angle;
    size_t start_pos;
    bool cmd;
    bool dir;

    bool (*command_rotate)(MotorRotoationCtrler* controller, double degrees);
    bool (*update)(MotorRotoationCtrler* controller);
} MotorRotoationCtrler;

void QEnc_Init(QEnc* qenc, MotorRotoationCtrler* params);
size_t get_ticks(QEnc* qenc);
size_t increment(QEnc* qenc);
size_t decrement(QEnc* qenc);
