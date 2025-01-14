/**
 * 
 * 
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct Flash Flash;

struct Flash
{
    bool (*write) (Flash *flash, size_t addr, uint32_t data);
    bool (*eraseSector) (Flash *flash, size_t sector);

    void *priv;
};
