
#pragma once

#include "w25q.h"
#include <stdio.h>
#include <string.h>

#define MOCK_PAGE_SIZE_BYTES 16
#define MOCK_SECTOR_SIZE_BYTES 256

void MockW25qInit(W25q *flash);
bool MockW25qRead(W25q *flash, size_t address, uint8_t *data, size_t size);
bool MockW25qPageWrite(W25q *flash, size_t address, uint8_t *data, size_t size);
bool MockW25qSectorErase(W25q *flash, size_t address);
