
#pragma once

#include <stdbool.h>
#include "matrix.h"

typedef struct Kalman Kalman;

typedef struct
{
    Matrix* A;
    Matrix* H;
    Matrix* Q;
    Matrix* R;
    // Matrix* (*transition)(Kalman* k, Matrix* u, Matrix* xp);
    // void* priv;
} StateTransition;

struct Kalman
{
    Matrix* x;
    Matrix* P;
    StateTransition t_model;
};

bool predict(Kalman* k);
bool estimate(Kalman* k, Matrix* z);
