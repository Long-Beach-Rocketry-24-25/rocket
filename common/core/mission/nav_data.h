
#pragma once

#include <stdint.h>
#include <stdbool.h>

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

typedef struct
{
    double w;
    double x;
    double y;
    double z;
} QuaternionVec;


typedef struct NavData NavData;

struct NavData
{
    float pressure;
    ThreeAxisVec accel;
    EulerVec euler;
    float temperature;
    bool (*update) (NavData *data);
    void *priv;
};
