/**
 * 
 * 
 */

#pragma once

#include "i2c.h"

#define TMP102_ADDR_GND 0
#define TMP102_ADDR_VDD 1
#define TMP102_ADDR_SDA 2
#define TMP102_ADDR_SCL 3

typedef struct Tmp102 Tmp102;

struct Tmp102
{
    I2c* bus;
    uint8_t addr;
    bool status;
    float (*get_temp_c)(Tmp102* tmp);
};

/**
 * You MUST call init for a given device before using any
 * other functions.
 */
bool Tmp102_Init(Tmp102* dev, I2c* bus, uint8_t dev_addr);
float Tmp102_Get_Temp_C(Tmp102* dev);
