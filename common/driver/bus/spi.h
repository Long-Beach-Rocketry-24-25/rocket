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
    bool (*transact) (Spi *spi, uint8_t *txdata, uint8_t *rxdata, size_t size);
    void *priv;
};

typedef struct ChipSelect ChipSelect;

struct ChipSelect
{
    bool (*select) (ChipSelect * cs);
    bool (*deselect) (ChipSelect *cs);
    void * priv;
};