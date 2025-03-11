
#include "stalloc.h"

static uint8_t* static_mem;
static size_t mem_size;
static size_t idx;
static bool inited = false;

bool InitStalloc(Mem* memory, uint8_t* data, size_t size)
{
    memory->alloc = Stalloc;
    if (!inited)
    {
        static_mem = data;
        mem_size = size;
        idx = 0;
        inited = true;
        return true;
    }
    return false;
}

void* Stalloc(size_t size)
{
    if (!inited || ((idx + size) > mem_size))
    {
        return NULL;
    }
    uint8_t* curr_ptr = static_mem + idx;
    idx += size;
    return (void*)curr_ptr;
}