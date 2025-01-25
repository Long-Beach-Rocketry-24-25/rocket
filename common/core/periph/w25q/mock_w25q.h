
#pragma once

#include "w25q.h"
#include "send.h"

#include <string.h>

#define MOCK_PAGE_SIZE_BYTES 256
#define MOCK_SECTOR_SIZE_BYTES 4096
#define MOCK_W25Q_MEM_SIZE MOCK_SECTOR_SIZE_BYTES * 2

void MockW25qInit(W25q *flash);
bool MockW25qRead(W25q *flash, size_t address, uint8_t *data, size_t size);
bool MockW25qPageWrite(W25q *flash, size_t address, uint8_t *data, size_t size);
bool MockW25qSectorErase(W25q *flash, size_t address);
bool MockW25qChipErase(W25q *flash);
void MockW25qDumpMem(Send *sender);
