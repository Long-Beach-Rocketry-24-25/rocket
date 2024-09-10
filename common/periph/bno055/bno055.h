/**
 * 
 * 
 */

#pragma once

#include "i2c.h"

#define BNO055_DEV_ADDR                 (0x28 << 1)
#define BNO055_IMU_MODE                 0x8

#define BNO055_OPR_MODE_REG             0x3D
#define BNO055_EULER_START_REG          0x1A
#define BNO055_QUATERNION_START_REG     0x20
#define BNO055_TEMP_REG                 0x34

typedef struct Bno055 Bno055;

typedef struct
{
    int16_t x;
    int16_t y;
    int16_t z;
} EulerVec;

typedef struct
{
    double w;
    double x;
    double y;
    double z;
} QuaternionVec;


struct Bno055
{
    I2c *bus;
    uint8_t addr;
    bool status;
    void (*get_euler) (Bno055 *dev, EulerVec* vec);
    void (*get_quaternion) (Bno055 *dev, QuaternionVec* vec);
    uint8_t (*get_temp_c) (Bno055 *dev);
};

bool Bno055_Init(Bno055 *dev, I2c *bus);
void Bno055_Set_Mode(Bno055 *dev, uint8_t mode);
void Bno055_Get_Euler(Bno055 *dev, EulerVec *vec);
void Bno055_Get_Quaternion(Bno055 *dev, QuaternionVec *vec);
uint8_t Bno055_Get_Temp_C(Bno055 *dev);
