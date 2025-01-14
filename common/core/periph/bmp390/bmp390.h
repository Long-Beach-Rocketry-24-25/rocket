/**
 * 
 * 
 */


#pragma once

#include <stdint.h>

#include "i2c.h"

#define BMP390_DEV_ADDR                     (0x76 << 1)

#define BMP390_CALIB_REG_START              (0x31)
#define BMP390_CALIB_REG_LEN                (21)

#define BMP390_PWR_CTRL_REG                 (0x1B)
#define BMP390_PWR_CTRL_PRESS_EN            (0x1)
#define BMP390_PWR_CTRL_TEMP_EN             (0x2)
#define BMP390_PWR_CTRL_MODE                (0x3 << 4)

#define BMP390_PRESS_DAT_REG_START          (0x4)
#define BMP390_TEMP_DAT_REG_START           (0x7)

typedef struct 
{
    uint16_t t1;
    uint16_t t2;
    int8_t t3;
    int16_t p1;
    int16_t p2;
    int8_t p3;
    int8_t p4;
    uint16_t p5;
    uint16_t p6;
    int8_t p7;
    int8_t p8;
    int16_t p9;
    int8_t p10;
    int8_t p11;
    int64_t t_fine;
} Bmp390CalibData;

typedef struct Bmp390 Bmp390;

struct Bmp390
{
    I2c *bus;
    uint8_t addr;
    bool status;
    Bmp390CalibData cal;
    float last_press;
    float last_temp;
    float (*get_pressure_pa) (Bmp390* dev);
    float (*get_temp_c) (Bmp390* dev);
};

/**
 * You MUST call init/config for a given device before using any
 * other functions.
 */
void Bmp390_Init(Bmp390* dev, I2c* i2c, uint8_t addr);
void Bmp390_Config(Bmp390* dev);
float Bmp390_Get_Pressure(Bmp390* dev);
float Bmp390_Get_Temp_C(Bmp390* dev);