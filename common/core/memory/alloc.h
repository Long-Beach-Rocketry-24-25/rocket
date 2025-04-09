
#pragma once

#include <stdlib.h>

#define ALLOC(mem, type) (type*)(mem)->alloc(sizeof(type))
#define ALLOC_N(mem, type, num) (type*)(mem)->alloc(sizeof(type) * num)

typedef struct Mem Mem;

struct Mem
{
    void* (*alloc)(size_t size);
};
