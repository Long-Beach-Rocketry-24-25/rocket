
#pragma once

#include "encoder.h"
#include "exit.h"
#include "motor.h"

#include <stdbool.h>
#include <stddef.h>

typedef enum
{
    IDLE,
    ROTATING,
    DONE
} MotorRotationState;

typedef struct MotorRotater MotorRotater;
struct MotorRotater
{
    bool cmd;
    float cmd_degrees;
    size_t last_count;
    size_t target_distance;
    size_t curr_distance;
    MotorRotationState state;
    MotorDirection direction;

    size_t encoder_counts_per_rotation;
    Motor* motor;
    Encoder* encoder;
};

void MotorRotaterInit(MotorRotater* control, Motor* motor, Encoder* encoder,
                      size_t encoder_counts_per_rotation);
void MotorRotaterCommand(MotorRotater* control, bool cmd, float degrees);
bool MotorRotaterUpdate(MotorRotater* control);
bool MotorRotaterIsActive(MotorRotater* control);
