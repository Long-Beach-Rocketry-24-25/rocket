
#include "st_gpio_fctry.h"

bool MakeStGpio(Gpio* gpio, Mem* mem, StGpioParams params)
{
    StGpioParams* p = ALLOC(mem, StGpioParams);
    EXIT_IF(p == NULL, false);

    *p = params;

    StGpioInit(gpio, p);
    StGpioConfig(gpio);

    return true;
}
