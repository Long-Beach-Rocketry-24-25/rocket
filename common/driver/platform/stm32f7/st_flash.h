/**
 *
 *
 */

#pragma once

#include "stm32f7xx.h"
#include "st_flash_defs.h"
#include "flash.h"
#include "timeout.h"

typedef enum
{
    X8 = 0,  // 1.8-2.1V
    X16, // 2.1-2.4V
    X32, // 2.7-3.6V
    X64  // 2.7-3.6V + Vpp
} StFlashPSize;

typedef struct
{
    FLASH_TypeDef *instance;
    Timeout *timer;

} StPrivFlash;

void StFlashInit(Flash *flash, StPrivFlash *st_flash, uint32_t base_addr, Timeout* timer);
bool StFlashWrite(Flash *flash, size_t addr, uint32_t data);
bool StFlashEraseSector(Flash *flash, size_t sector);
