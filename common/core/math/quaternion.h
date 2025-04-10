/**
 * Quaternion math library.
 */

#pragma once

#include "matrix.h"

typedef struct
{
    float w;
    float x;
    float y;
    float z;
} QuaternionVec;

QuaternionVec* quat_multiply(QuaternionVec* q1, QuaternionVec* q2,
                             QuaternionVec* out);
QuaternionVec* quat_conjugate(QuaternionVec* quat, QuaternionVec* out);
bool quat_compare(QuaternionVec* q1, QuaternionVec* q2);
