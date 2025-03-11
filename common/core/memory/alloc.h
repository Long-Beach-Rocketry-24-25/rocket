
#pragma once

#include <stdlib.h>

#define ALLOC(mem, type) (type*)(mem)->alloc(sizeof(type))

typedef struct Mem Mem;

struct Mem
{
    void* (*alloc)(size_t size);
};
