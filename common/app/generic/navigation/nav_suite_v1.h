
#pragma once

#include "nav_data.h"

#include "bmp390.h"
#include "bno055.h"

typedef struct
{
    Bmp390* baro;
    Bno055* imu;
} NavSuiteV1;

void NavSuiteV1Init(NavData* data, NavSuiteV1* bosch, Bmp390* baro,
                    Bno055* imu);
bool NavSuiteV1Update(NavData* data);
