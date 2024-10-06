

#pragma once

#include "gpio.h"
#include "i2c.h"
#include "st_i2c_def.h"
#include "timeout.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Inter-integrated Circuit Interface
 */
typedef struct
{
    volatile uint32_t CR1;         /*!< I2C Control register 1,            Address offset: 0x00 */
    volatile uint32_t CR2;         /*!< I2C Control register 2,            Address offset: 0x04 */
    volatile uint32_t OAR1;        /*!< I2C Own address 1 register,        Address offset: 0x08 */
    volatile uint32_t OAR2;        /*!< I2C Own address 2 register,        Address offset: 0x0C */
    volatile uint32_t TIMINGR;     /*!< I2C Timing register,               Address offset: 0x10 */
    volatile uint32_t TIMEOUTR;    /*!< I2C Timeout register,              Address offset: 0x14 */
    volatile uint32_t ISR;         /*!< I2C Interrupt and status register, Address offset: 0x18 */
    volatile uint32_t ICR;         /*!< I2C Interrupt clear register,      Address offset: 0x1C */
    volatile uint32_t PECR;        /*!< I2C PEC register,                  Address offset: 0x20 */
    volatile uint32_t RXDR;        /*!< I2C Receive data register,         Address offset: 0x24 */
    volatile uint32_t TXDR;        /*!< I2C Transmit data register,        Address offset: 0x28 */
} ST_I2C_TypeDef;

typedef struct
{
    ST_I2C_TypeDef * instance;
    Gpio sda;
    Gpio scl;
    Timeout * timer;
} StPrivI2c;

void St_I2c_Init(I2c *i2c, StPrivI2c *st_i2c, uint32_t base_addr, Timeout* timer);

void St_I2c_Config(I2c *i2c, uint32_t timingr);

bool St_I2c_SetTarget(I2c *i2c, uint8_t dev_addr);

bool St_I2c_Write(I2c *i2c, uint16_t addr, uint8_t *data, size_t size);

bool St_I2c_Read(I2c *i2c, uint16_t addr, uint8_t *data, size_t size);
