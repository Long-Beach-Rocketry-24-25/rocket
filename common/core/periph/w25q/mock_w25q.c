
#include "mock_w25q.h"

#define FAKE_MEM_SIZE MOCK_SECTOR_SIZE_BYTES * 2

static uint8_t memory[FAKE_MEM_SIZE] = {0};

void MockW25qInit(W25q *flash)
{
    flash->page_write = MockW25qPageWrite;
    flash->read = MockW25qRead;
    flash->erase_sector = MockW25qSectorErase;
    flash->page_size = MOCK_PAGE_SIZE_BYTES;
    flash->sector_size = MOCK_SECTOR_SIZE_BYTES;

    flash->read = MockW25qRead;
    flash->page_write = MockW25qPageWrite;
    flash->erase_sector = MockW25qSectorErase;

    memset(memory, 0xFF, FAKE_MEM_SIZE);
}

bool MockW25qRead(W25q *flash, size_t address, uint8_t *data, size_t size)
{
    for (size_t i = 0; i < size && i < FAKE_MEM_SIZE; ++i)
    {
        data[i] = memory[address + i];
    }

    return true;
}

bool MockW25qPageWrite(W25q *flash, size_t address, uint8_t *data, size_t size)
{
    size_t page_begin = get_section_addr(address, flash->page_size);
    size_t index = address;
    for (size_t i = 0; i < size; ++i)
    {
        memory[index] = data[i];
        index = (index >= page_begin + flash->page_size) ? page_begin : index + 1;
    }

    return true;
}

bool MockW25qSectorErase(W25q *flash, size_t address)
{
    size_t sector_begin = get_section_addr(address, flash->sector_size);
    memset(memory, 0xFF, flash->sector_size);
    return true;
}
