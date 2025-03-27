
#pragma once

#include "quaternion.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct
{
    float x;
    float y;
    float z;
} ThreeAxisVec;

typedef struct
{
    int16_t x;
    int16_t y;
    int16_t z;
} EulerVec;

typedef struct NavData NavData;

struct NavData
{
    float pressure;
    ThreeAxisVec accel;
    QuaternionVec quat;
    float temperature;
    size_t tick;
    bool (*update)(NavData* data);
    void* priv;
};
