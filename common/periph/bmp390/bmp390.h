/**
 * Bmp390 Interface.
 * 
 */

#pragma once

typedef struct Bmp390 Bmp390;

struct Bmp390
{
    float (*get_pressure_pa) (Bmp390* dev);
    float (*get_temp_c) (Bmp390* dev);
    void * priv;
};
