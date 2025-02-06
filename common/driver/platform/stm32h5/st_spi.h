
#pragma once

#include "stm32h5xx_hal.h"

#include "gpio.h"
#include "spi.h"
#include "timeout.h"

typedef struct
{
    SPI_HandleTypeDef handle;
    Gpio mosi;
    Gpio miso;
    Gpio scl;
    uint32_t timeout_ms;
} StPrivSpi;

void StSpiInit(Spi* spi, StPrivSpi* st_spi, uint32_t base_addr,
               uint32_t timeout_ms);
void StSpiConfig(Spi* spi);
bool StSpiSend(Spi* spi, uint8_t* data, size_t size);
bool StSpiRead(Spi* spi, uint8_t* data, size_t size);
bool StSpiTransact(Spi* spi, uint8_t* txdata, uint8_t* rxdata, size_t size);
