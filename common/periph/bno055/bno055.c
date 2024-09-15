
#include "bno055.h"


static inline int16_t form_signed16(uint8_t msb, uint8_t lsb)
{
    return ((int16_t) msb << 8) | lsb;
}

bool Bno055_Init(Bno055 *dev, I2c *bus, uint8_t addr)
{
    dev->bus = bus;
    dev->addr = addr;
    dev->get_accel = Bno055_Get_Accel;
    dev->get_euler = Bno055_Get_Euler;
    dev->get_quaternion = Bno055_Get_Quaternion;
    dev->get_temp_c = Bno055_Get_Temp_C;
    dev->status = true;

    return true;
}

void Bno055_Set_Mode(Bno055 *dev, uint8_t mode)
{
    dev->status = dev->bus->set_target(dev->bus, dev->addr);
    dev->status = dev->bus->write(dev->bus, BNO055_OPR_MODE_REG, &mode, 1);
}

void Bno055_Get_Accel(Bno055 *dev, ThreeAxisVec *vec)
{
    uint8_t data[6] = {0};
    dev->status = dev->bus->set_target(dev->bus, dev->addr);
    dev->status = dev->bus->read(dev->bus, BNO055_ACCEL_START_REG, data, 6);

    vec->x = form_signed16(data[1], data[0]) / 100.0;
    vec->y = form_signed16(data[3], data[2]) / 100.0;
    vec->z = form_signed16(data[5], data[4]) / 100.0;
}

void Bno055_Get_Euler(Bno055 *dev, EulerVec *vec)
{
    uint8_t data[6] = {0};
    dev->status = dev->bus->set_target(dev->bus, dev->addr);
    dev->status = dev->bus->read(dev->bus, BNO055_EULER_START_REG, data, 6);

    /*
     * Scale is 1 / 16.
     */
    vec->x = form_signed16(data[1], data[0]) >> 4;
    vec->y = form_signed16(data[3], data[2]) >> 4;
    vec->z = form_signed16(data[5], data[4]) >> 4;
}

void Bno055_Get_Quaternion(Bno055 *dev, QuaternionVec *vec)
{
    uint8_t data[8] = {0};
    dev->status = dev->bus->set_target(dev->bus, dev->addr);
    dev->status = dev->bus->read(dev->bus, BNO055_QUATERNION_START_REG, data, 8);

    double quat_fact = 1. / (1 << 14);
    vec->w = form_signed16(data[1], data[0]) * quat_fact;
    vec->x = form_signed16(data[3], data[2]) * quat_fact;
    vec->y = form_signed16(data[5], data[4]) * quat_fact;
    vec->z = form_signed16(data[7], data[6]) * quat_fact;
}

uint8_t Bno055_Get_Temp_C(Bno055 *dev)
{
    uint8_t data = 0;
    dev->status = dev->bus->set_target(dev->bus, dev->addr);
    dev->status = dev->bus->read(dev->bus, BNO055_TEMP_REG, &data, 1);

    return data;
}