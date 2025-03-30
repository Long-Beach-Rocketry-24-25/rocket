
#pragma once

#include "nav_data.h"

ThreeAxisVec* accel_rotate(ThreeAxisVec* accel, QuaternionVec* orientation,
                           ThreeAxisVec* result);

double accel_to_velocity(double accel, double time_s);