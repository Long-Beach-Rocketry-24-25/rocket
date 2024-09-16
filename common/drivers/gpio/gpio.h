/**
 * 
 * 
 */

#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef struct Gpio Gpio;

struct Gpio
{
    bool (*toggle) (Gpio* gpio);
    void (*set) (Gpio* gpio, bool active);
    bool (*read) (Gpio* gpio);
    void* priv;
};