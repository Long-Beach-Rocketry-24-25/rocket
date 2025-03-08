#pragma once

#include <stdbool.h>
#include "qenc.h"
#include "st_gpio.h"
#include "st_pwm.h"
#include "timeout.h"

typedef enum
{
    START = 0,
    RUNNING,
    IDLE,

} StDCState;

typedef struct
{
    DCMotor motor;
    uint16_t encoder_ticks;
    QEnc encoder;
    StGpioParams motor_pin_1;
    StGpioParams motor_pin_2;
    StGpioParams motor_pin_3;

} DCPosControl;

typedef struct DCMotor DCMotor;

struct DCMotor
{
    StDCState state;

    bool (*set_en)(DCMotor* motor, bool enable, Gpio* gpio, Pwm* pwm);
    bool (*set_direction)(DCMotor* motor, bool direction);
    void (*set_duty)(DCMotor* motor, float duty);

    void* priv;
};
