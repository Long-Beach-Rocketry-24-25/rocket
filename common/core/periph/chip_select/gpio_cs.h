
#pragma once

#include "gpio.h"
#include "spi.h"

typedef struct
{
    bool activeLow;
    Gpio pin;
} GpioChipSelect;

void GpioCsInit(ChipSelect* cs, GpioChipSelect* gpio_cs, bool activeLow);
void GpioCsConfig(ChipSelect* cs);
bool GpioCsSelect(ChipSelect* cs);
bool GpioCsDeselect(ChipSelect* cs);
