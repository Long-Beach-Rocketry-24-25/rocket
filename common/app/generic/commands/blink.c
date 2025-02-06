
#include "blink.h"

static Send* send;
static Gpio* gpio;

void init_blink(Send* sender, Gpio* pin)
{
    gpio = pin;
    send = sender;
}

void blink(int argc, char* argv[])
{
    send->fwrite(send, "Blink - %d", gpio->toggle(gpio));
}
