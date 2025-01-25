
#pragma once

#include "spi.h"

#define W25Q_PAGE_SIZE_BYTES 256
#define W25Q_SECTOR_SIZE_BYTES 4096

typedef struct W25q W25q;

struct W25q
{
    Spi *bus;
    bool (*page_write) (W25q* flash, size_t address, uint8_t *data, size_t size);
    bool (*read) (W25q *flash, size_t address, uint8_t *data, size_t size);
    bool (*erase_sector) (W25q *flash, size_t address);
    size_t page_size;
    size_t sector_size;
    size_t mem_size;
};

void W25qInit(W25q *flash, Spi *spi, size_t mem_size);
bool W25qRead(W25q *flash, size_t address, uint8_t *data, size_t size);
bool W25qPageWrite(W25q *flash, size_t address, uint8_t *data, size_t size);
bool W25qSectorErase(W25q *flash, size_t address);
size_t get_section_addr(size_t address, size_t section_size);
