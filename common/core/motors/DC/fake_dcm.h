#pragma once

#include "dcm.h"
#include "pwm.h"
#include "qenc.h"

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
    bool enabled;
    bool disabled;
    bool direction_one;
    bool direction_two;
    float duty;
    Pwm* pwm;

} DCPosControl;

void Drv8242Init(DCMotor* motor, DCPosControl* control, Pwm* pwm);
bool Drv8242SetEnable(DCMotor* motor, bool enable);
bool Drv8242SetDir(DCMotor* motor, bool direction);
void Drv8242SetDuty(DCMotor* motor, float duty);
