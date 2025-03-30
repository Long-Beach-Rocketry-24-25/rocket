
#pragma once

#include "acceleration.h"
#include "altitude.h"
#include "nav_data.h"
#include "navigator.h"
#include "simple_kalman.h"

// todo: error propagation? nav data doesn't report errors rn :(

typedef struct
{
    NavData* data;
    SimpleKalman kalman;
    double base_pressure;
    bool started;
} Helios;

bool HeliosNavigatorInit(Navigator* nav, Helios* helios, NavData* nav_data);
// bool HeliosKalmanInit(Matrix* A, Matrix* H, double timestep);
bool HeliosConfig(Navigator* nav, Matrix* x, Matrix* P, Matrix* A, Matrix* H,
                  Matrix* Q, Matrix* R);
bool HeliosStart(Navigator* nav);
bool HeliosUpdate(Navigator* nav);
double HeliosAltitude(Navigator* nav);
double HeliosVelocity(Navigator* nav);
const QuaternionVec* HeliosOrientation(Navigator* nav);
