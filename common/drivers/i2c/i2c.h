/**
 * 
 * 
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct I2c I2c;

struct I2c
{
    bool (*set_target) (I2c *i2c, uint8_t dev_addr);
    bool (*write) (I2c *i2c, uint16_t addr, uint8_t *data, size_t size);
    bool (*read) (I2c *i2c, uint16_t addr, uint8_t *data, size_t size);

    void *priv;
};