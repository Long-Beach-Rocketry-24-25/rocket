
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "alloc.h"

void InitDalloc(Mem* mem);
void SetEnableDalloc(bool enable);
void* Dalloc(size_t size);
