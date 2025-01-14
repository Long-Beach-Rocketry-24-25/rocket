/**
 * 
 * 
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct Spi Spi;

struct Spi
{
    bool (*select_target) (Spi *spi);
    bool (*write) (Spi *spi, uint8_t *data, size_t size);
    bool (*read) (Spi *spi, uint8_t *data, size_t size);
    bool (*write_and_read) (Spi *spi, uint8_t *txdata, uint8_t *rxdata, size_t size);

    void *priv;
};