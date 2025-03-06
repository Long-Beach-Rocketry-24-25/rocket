

#pragma once

#include "stm32l4xx.h"

#include "dcm.h"
#include "gpio.h"
#include "pwm.h"
#include "qenc.h"
#include "timeout.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum
{
    START = 0,
    RUNNING,
    IDLE,

} StDCState;

typedef struct
{
    uint16_t encoder_ticks;
    DCM_TypeDef* instance;

} StDCPriv;

typedef struct
{
    StDCPriv priv;
    QEnc encoder;

} StDCPosControl;

bool StDCMotorEnable(DCMotor* motor, bool enable);
bool StDCMotorSetPos(DCMotor* motor, bool position);
void StDCMotorSetDuty(DCMotor* motor, float duty);
