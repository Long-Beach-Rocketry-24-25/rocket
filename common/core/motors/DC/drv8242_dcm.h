

#pragma once

#include "dcm.h"
#include "gpio.h"
#include "pwm.h"
#include "qenc.h"
#include "timeout.h"

#include <stdbool.h>

typedef enum
{
    START = 0,
    RUNNING,
    IDLE,
    OFF

} DCState;

typedef struct
{
    DCMotor* instance;
    DCState state;
    uint16_t encoder_ticks;
    size_t angle;
    bool cmd;
    Pwm* pwm;
    Gpio* nSleep_pin;
    Gpio* drvoff_pin;
    Gpio* en_in1_pin;
    Gpio* ph_in2_pin;

} DCPosControl;

void Drv8242Init(DCMotor* motor, DCPosControl* control, Gpio* nSleep_pwm,
                 Gpio* drvoff_pin, Gpio* en_in1_pin, Gpio* ph_in2_pin,
                 Pwm* pwm);
bool Drv8242SetEnable(DCMotor* motor, bool enable);
bool Drv8242SetDir(DCMotor* motor, bool direction);
void Drv8242SetDuty(DCMotor* motor, float duty);
