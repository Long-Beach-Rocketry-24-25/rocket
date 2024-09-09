
#include "bno055.h"


bool Bno055_Init(Bno055 *dev, I2c *bus)
{
    dev->bus = bus;
    dev->get_euler = Bno055_Get_Euler;
    dev->get_temp_c = Bno055_Get_Temp_C;
    dev->status = true;

    return true;
}

void Bno055_Set_Mode(Bno055 *dev, uint8_t mode)
{
    dev->status = dev->bus->set_target(dev->bus, BNO055_DEV_ADDR);
    dev->status = dev->bus->write(dev->bus, BNO055_OPR_MODE_REG, &mode, 1);
}

void Bno055_Get_Euler(Bno055 *dev, EulerVec *vec)
{
    uint8_t data[6] = {0};
    dev->status = dev->bus->set_target(dev->bus, BNO055_DEV_ADDR);
    dev->status = dev->bus->read(dev->bus, BNO055_EULER_START_REG, data, 6);

    vec->x = (((int16_t) (data[1] << 8)) | ((int16_t) data[0])) >> 4;
    vec->y = (((int16_t) (data[3] << 8)) | ((int16_t) data[2])) >> 4;
    vec->z = (((int16_t) (data[5] << 8)) | ((int16_t) data[4])) >> 4;
}

uint8_t Bno055_Get_Temp_C(Bno055 *dev)
{
    uint8_t data = 0;
    dev->status = dev->bus->set_target(dev->bus, BNO055_DEV_ADDR);
    dev->status = dev->bus->read(dev->bus, BNO055_TEMP_REG, &data, 1);

    return data;
}