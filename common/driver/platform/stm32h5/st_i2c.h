

#pragma once

#include "stm32h5xx.h"

#include "gpio.h"
#include "i2c.h"
#include "timeout.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct
{
    I2C_TypeDef * instance;
    Gpio sda;
    Gpio scl;
    Timeout * timer;
} StPrivI2c;

void StI2cInit(I2c *i2c, StPrivI2c *st_i2c, uint32_t base_addr, Timeout* timer);
void StI2cConfig(I2c *i2c, uint32_t timingr);
bool StI2cSetTarget(I2c *i2c, uint8_t dev_addr);
bool StI2cWrite(I2c *i2c, uint16_t addr, uint8_t *data, size_t size);
bool StI2cRead(I2c *i2c, uint16_t addr, uint8_t *data, size_t size);
