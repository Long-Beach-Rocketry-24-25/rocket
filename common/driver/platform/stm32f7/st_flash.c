
#include "st_flash.h"

#define FLASH_KEY_1 0x45670123
#define FLASH_KEY_2 0xCDEF89AB

static bool set_flash_lock(StPrivFlash *flash, bool lock)
{
    if (lock)
    {
        flash->instance->CR |= FLASH_CR_LOCK;
    }
    else
    {
        flash->instance->KEYR = FLASH_KEY_1;
        flash->instance->KEYR = FLASH_KEY_2;
    }

    return flash->instance->CR & FLASH_CR_LOCK;
}

static inline bool is_busy(StPrivFlash *flash)
{
    return flash->instance->SR & FLASH_SR_BSY;
}

static inline void program_word(size_t addr, uint32_t data)
{
    *(volatile uint32_t *)addr = data;
}

static inline void program_byte(size_t addr, uint8_t data)
{
    *(volatile uint8_t *)addr = data;
}

void StFlashInit(Flash *flash, StPrivFlash *st_flash, uint32_t base_addr, Timeout* timer)
{
    st_flash->instance = (FLASH_TypeDef *) base_addr;
    st_flash->timer = timer;

    flash->priv = (void *) st_flash;
    flash->write = StFlashWrite;
    flash->eraseSector = StFlashEraseSector;
}

bool StFlashWrite(Flash *flash, size_t addr, uint32_t data)
{
    StPrivFlash * dev = (StPrivFlash *) flash->priv;

    // Check bsy bit in flash reg
    if (is_busy(dev) || set_flash_lock(dev, false))
    {
        return false;
    }

    // Set PG bit in flash cr
    dev->instance->CR |= FLASH_CR_PG;
    dev->instance->CR |= (X32 & 0x3) << FLASH_CR_PSIZE_Pos;
    // Perform writes in memory area
    program_word(addr, data);

    __DSB();

    // Wait for BSY to be clear
    WAIT(dev->timer, !is_busy(dev), false);

    dev->instance->CR &= ~FLASH_CR_PG;
    dev->instance->CR &= ~FLASH_CR_PSIZE;

    set_flash_lock(dev, true);

    // Check SR
    if (dev->instance->SR)
    {
        dev->instance->SR |= 0xF3;
        return false;
    }

    return true;
}

bool StFlashEraseSector(Flash *flash, size_t sector)
{
    if (sector > MAX_SECTOR)
    {
        return false;
    }

    StPrivFlash * dev = (StPrivFlash *) flash->priv;

    // Check bsy bit in flash reg
    if (is_busy(dev) || set_flash_lock(dev, false))
    {
        return false;
    }

    // Set SER bit and select sector in SNB in flash CR
    dev->instance->CR |= FLASH_CR_SER;
    dev->instance->CR &= ~FLASH_CR_SNB;
    dev->instance->CR |= (sector & 0xF) << FLASH_CR_SNB_Pos;
    dev->instance->CR |= FLASH_CR_STRT;

    __DSB();

    dev->instance->CR &= ~FLASH_CR_SER;
    dev->instance->CR &= ~FLASH_CR_SNB;

    // Wait for bsy bit to be cleared
    WAIT(dev->timer, !is_busy(dev), false);

    set_flash_lock(dev, true);

    // Check SR
    if (dev->instance->SR)
    {
        dev->instance->SR |= 0xF3;
        return false;
    }

    return true;
}
