
#pragma once

#include "spi.h"

typedef struct W25q W25q;

struct W25q
{
    Spi *bus;
    bool (*page_write) (W25q* flash, size_t address, uint8_t *data, size_t size);
    bool (*read) (W25q *flash, size_t address, uint8_t *data, size_t size);
    bool (*erase_sector) (W25q *flash, size_t address);
};

void W25qInit(W25q *flash, Spi *spi);
bool W25qRead(W25q *flash, size_t address, uint8_t *data, size_t size);
bool W25qPageWrite(W25q *flash, size_t address, uint8_t *data, size_t size);
bool W25qSectorErase(W25q *flash, size_t address);