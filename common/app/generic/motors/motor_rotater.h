
#pragma once

#include "encoder.h"
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
bool MotorRotaterUpdate(MotorRotater* control, bool cmd, float degrees);
