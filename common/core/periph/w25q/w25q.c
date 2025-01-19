
#include "w25q.h"

// static inline getpageaddr and getsectoraddr

static inline size_t get_page_addr(size_t address, size_t page_size)
{
    return address - (address % page_size);
}

static inline size_t get_sector_addr(size_t address, size_t sector_size)
{
    return address - (address % sector_size);
}

static bool mask_status(W25q *flash, uint8_t cmd, uint8_t mask)
{
    // Chip select must already be asserted prior to function call.
    uint8_t status = 0;
    flash->bus->send(flash->bus, &cmd, 1);
    flash->bus->read(flash->bus, &status, 1);
    return status & mask;
}

static void wait_busy(W25q *flash)
{
    // Wait busy bit in status register 1, 0x5
    flash->bus->cs.select(&flash->bus->cs);
    while (mask_status(flash, 0x5, 1));
    flash->bus->cs.deselect(&flash->bus->cs);
}

static void write_enable(W25q *flash)
{
    // Write enable instruction 0x6
    uint8_t write_en = 0x6;
    flash->bus->cs.select(&flash->bus->cs);
    flash->bus->send(flash->bus, &write_en, 1);
    flash->bus->cs.deselect(&flash->bus->cs);
}

void W25qInit(W25q *flash, Spi *spi)
{
    flash->bus = spi;
    flash->page_write = W25qPageWrite;
    flash->read = W25qRead;
    flash->erase_sector = W25qSectorErase;
}

bool W25qPageWrite(W25q *flash, size_t address, uint8_t *data, size_t size)
{
    // Enforce start addr + size < getpageaddr + 256

    write_enable(flash);

    // Prog Page 0x2 + 24 bit address
    uint8_t txbuf[4] = {0x2, address >> 16, (address >> 8) & 0xFF, address & 0xFF};
    uint8_t rxbuf[256] = {0};
    flash->bus->cs.select(&flash->bus->cs);
    flash->bus->send(flash->bus, txbuf, 4);
    flash->bus->transact(flash->bus, data, rxbuf, size);
    flash->bus->cs.deselect(&flash->bus->cs);

    // Wait busy bit in status register 1, 0x5
    wait_busy(flash);

    return true;
}

bool W25qRead(W25q *flash, size_t address, uint8_t *data, size_t size)
{
    wait_busy(flash);

    // Write enable instruction 0x6
    uint8_t write_en = 0x6;
    flash->bus->cs.select(&flash->bus->cs);
    flash->bus->send(flash->bus, &write_en, 1);
    flash->bus->cs.deselect(&flash->bus->cs);

    // Read data instruction 0x3
    uint8_t txbuf[4] = {0x3, address >> 16, (address >> 8) & 0xFF, address & 0xFF};
    flash->bus->cs.select(&flash->bus->cs);
    flash->bus->send(flash->bus, txbuf, 4);
    flash->bus->read(flash->bus, data, size);
    flash->bus->cs.deselect(&flash->bus->cs);

    return true;
}

bool W25qSectorErase(W25q *flash, size_t address)
{
    address = get_sector_addr(address, 4096);

    write_enable(flash);

    // Write enable instruction 0x6, Sector erase cmd 0x20 followed by 24-bit address
    uint8_t txbuf[4] = {0x20, address >> 16, (address >> 8) & 0xFF, address & 0xFF};
    flash->bus->cs.select(&flash->bus->cs);
    flash->bus->send(flash->bus, txbuf, 4);
    flash->bus->cs.deselect(&flash->bus->cs);

    // Wait busy bit in status register 1, 0x5
    wait_busy(flash);

    return true;
}
