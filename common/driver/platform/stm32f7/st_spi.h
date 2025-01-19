
#pragma once

#include "stm32f746xx.h"
#include "stm32f7xx.h"

#include "spi.h"
#include "gpio.h"
#include "timeout.h"

typedef struct
{
    SPI_TypeDef * instance;
    Gpio mosi;
    Gpio miso;
    Gpio scl;
    ChipSelect cs;
    Timeout * timer;
} StPrivSpi;

void StSpiInit(Spi *spi, StPrivSpi *st_spi, uint32_t base_addr, Timeout *timer);
void StSpiConfig(Spi * spi);
bool StSpiTransact(Spi * spi, uint8_t *txdata, uint8_t *rxdata, size_t size);
