
#include "gpio_cs.h"

void GpioCsInit(ChipSelect *cs, GpioChipSelect *gpio_cs, bool activeLow)
{
    gpio_cs->activeLow = activeLow;
    cs->priv = (void *) gpio_cs;
    cs->select = GpioCsSelect;
    cs->deselect = GpioCsDeselect;
}

void GpioCsConfig(ChipSelect *cs)
{
    GpioChipSelect * dev = (GpioChipSelect *) cs->priv;
    dev->pin.config(&dev->pin);
    GpioCsDeselect(cs);
}

bool GpioCsSelect(ChipSelect *cs)
{
    GpioChipSelect * dev = (GpioChipSelect *) cs->priv;
    dev->pin.set(&dev->pin, !dev->activeLow);
    return true;
}

bool GpioCsDeselect(ChipSelect *cs)
{
    GpioChipSelect * dev = (GpioChipSelect *) cs->priv;
    dev->pin.set(&dev->pin, dev->activeLow);
    return true;
}