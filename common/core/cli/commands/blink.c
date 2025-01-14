
#include "blink.h"

static Gpio *gpio;

void init_blink(Gpio * pin)
{
    gpio = pin;
}

void blink(int argc, char* argv[])
{
    cli_write("Blink - %d", gpio->toggle(gpio));
}
