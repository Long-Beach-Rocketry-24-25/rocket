
#pragma once

#include <stdint.h>
#include "dcm.h"
#include "gpio.h"
#include "pwm.h"
#include "st_dcm.h"

typedef enum
{
    IDLE = 0,
    RUNNING,
    DONE

} DCState;

typedef struct DCM_Control DCM_Control;

struct DCM_Control
{
    DCState state;

    bool cmd;
    //0 for CCW 1 for CW
    bool dir;
    float cmd_degrees;
    size_t curr_pos;
    size_t prev_pos;
    size_t target_count;
    size_t pusle_per_rev;
    size_t diff;

    DCMotor* motor;
};

void DCM_Control_Init(DCM_Control* control, DCMotor* motor,
                      size_t pulse_per_rev);

void DcmControlCommand(DCM_Control* control, bool command, float degrees);

bool DcmControlUpdate(DCM_Control* control);
