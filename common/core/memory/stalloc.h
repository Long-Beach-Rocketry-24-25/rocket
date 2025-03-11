
#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "alloc.h"

bool InitStalloc(Mem* mem, uint8_t* data, size_t size);
void* Stalloc(size_t size);
