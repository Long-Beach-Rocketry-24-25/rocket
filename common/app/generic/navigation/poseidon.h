
#pragma once

#include "nav_data.h"
#include "navigator.h"
#include "simple_kalman.h"
// todo: implement quat -> rot mat
// todo: implement quat + accel 3ax -> vertical accel

typedef struct
{

} Poseidon;

bool PoseidonNavigatorInit(Navigator* nav, Poseidon* poseidon,
                           NavData* nav_data);