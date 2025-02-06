/**
 *
 *
 */

#pragma once

#include "ads111x_defs.h"
#include "i2c.h"

#include <stdint.h>

#define ADS111X_ADDR_GND 0
#define ADS111X_ADDR_VDD 1
#define ADS111X_ADDR_SDA 2
#define ADS111X_ADDR_SCL 3

typedef struct Ads111x Ads111x;

struct Ads111x
{
    I2c* bus;
    uint8_t addr;
    uint8_t gain;
    uint8_t mode;
    uint8_t datarate;
    bool status;
    void (*start_sample)(Ads111x* dev, uint8_t channel);
    int16_t (*read_sample)(Ads111x* dev);
    int16_t (*take_sample)(Ads111x* dev, uint8_t channel);
};

bool Ads111x_Init(Ads111x* dev, I2c* bus, uint8_t dev_addr);

void Ads111x_Config(Ads111x* dev, uint8_t gain, uint8_t mode, uint8_t datarate);

int16_t Ads111x_Sample_Channel(Ads111x* dev, uint8_t channel);

void Ads111x_Start_Sample(Ads111x* dev, uint8_t channel);

int16_t Ads111x_Read_Sample(Ads111x* dev);
