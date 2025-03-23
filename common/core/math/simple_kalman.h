
#pragma once

#include <stdbool.h>
#include "matrix.h"

typedef struct
{
    Matrix* A;
    Matrix* H;
    Matrix* Q;
    Matrix* R;
} SimpleKalmanConstants;

typedef struct
{
    Matrix* x;
    Matrix* P;
    SimpleKalmanConstants model;
} SimpleKalman;

bool SimpleKalmanPredict(SimpleKalman* k);
bool SimpleKalmanEstimate(SimpleKalman* k, Matrix* z);
