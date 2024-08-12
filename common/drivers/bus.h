/**
 * 
 * 
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef void (*write) (void * dev, uint8_t *data, size_t size);
typedef bool (*read) (void * dev, uint8_t *data, size_t size);

typedef struct
{
    write write;
    read read;
    void * dev;
    void * extra;
} Bus;