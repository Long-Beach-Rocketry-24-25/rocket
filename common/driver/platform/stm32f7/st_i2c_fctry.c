
#include "st_i2c_fctry.h"

bool MakeStI2c(I2c* i2c, Mem* mem, uint32_t base_addr, uint32_t timingr,
               Timeout* timer, StGpioParams io1, StGpioParams io2)
{

    StPrivI2c* st = ALLOC(mem, StPrivI2c);
    EXIT_IF(st == NULL, false);

    st->instance = (I2C_TypeDef*)base_addr;
    st->timer = timer;
    EXIT_IF(!MakeStGpio(&st->scl, mem, io1), false);
    EXIT_IF(!MakeStGpio(&st->sda, mem, io2), false);

    i2c->priv = (void*)st;
    i2c->write = StI2cWrite;
    i2c->read = StI2cRead;
    i2c->set_target = StI2cSetTarget;

    StI2cConfig(i2c, timingr);

    return true;
}