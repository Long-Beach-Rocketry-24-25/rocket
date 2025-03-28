
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
} Helios;

bool HeliosNavigatorInit(Navigator* nav, Helios* helios, NavData* nav_data);
// Reconfig
// Update