/**
 * Static memory implementation of alloc interface.
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "alloc.h"

/**
 * Initialize a memory allocation interface to use pre-allocated static memory.
 * 
 * @param mem the memory allocation interface.
 */
bool InitStalloc(Mem* mem, uint8_t* data, size_t size);

/**
 * Allocate memory for one-time use from pre-allocated static memory.
 * 
 * @param size the number of bytes to allocate.
 * @return a pointer to the start of allocated memory.
 */
void* Stalloc(size_t size);
