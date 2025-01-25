/**
 * 
 * 
 */

#pragma once

#include "i2c.h"
#include "nav_data.h"

#define BNO055_DEV_ADDR                 (0x28 << 1)
#define BNO055_IMU_MODE                 0x8

#define BNO055_OPR_MODE_REG             0x3D
#define BNO055_EULER_START_REG          0x1A
#define BNO055_QUATERNION_START_REG     0x20
#define BNO055_TEMP_REG                 0x34
#define BNO055_ACCEL_START_REG          0x8

typedef struct Bno055 Bno055;

struct Bno055
{
    I2c *bus;
    uint8_t addr;
    bool status;
    void (*get_accel) (Bno055 *dev, ThreeAxisVec *vec);
    void (*get_euler) (Bno055 *dev, EulerVec* vec);
    void (*get_quaternion) (Bno055 *dev, QuaternionVec* vec);
    uint8_t (*get_temp_c) (Bno055 *dev);
};

/**
 * You MUST call init/set mode for a given device before using any
 * other functions.
 */
bool Bno055_Init(Bno055 *dev, I2c *bus, uint8_t addr);
void Bno055_Set_Mode(Bno055 *dev, uint8_t mode);
void Bno055_Get_Accel(Bno055 *dev, ThreeAxisVec *vec);
void Bno055_Get_Euler(Bno055 *dev, EulerVec *vec);
void Bno055_Get_Quaternion(Bno055 *dev, QuaternionVec *vec);
uint8_t Bno055_Get_Temp_C(Bno055 *dev);
