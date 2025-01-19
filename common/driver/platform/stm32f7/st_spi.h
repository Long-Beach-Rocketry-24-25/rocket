
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
    Timeout * timer;
} StPrivSpi;

void StSpiInit(Spi *spi, StPrivSpi *st_spi, uint32_t base_addr, Timeout *timer);
void StSpiConfig(Spi * spi);
bool StSpiSend(Spi * spi, uint8_t *data, size_t size);
bool StSpiRead(Spi * spi, uint8_t *data, size_t size);
bool StSpiTransact(Spi * spi, uint8_t *txdata, uint8_t *rxdata, size_t size);
