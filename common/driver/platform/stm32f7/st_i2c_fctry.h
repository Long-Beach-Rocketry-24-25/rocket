
#pragma once

#include "alloc.h"
#include "exit.h"
#include "st_gpio_fctry.h"
#include "st_i2c.h"

bool MakeStI2c(I2c* i2c, Mem* mem, uint32_t base_addr, uint32_t timingr,
               Timeout* timer, StGpioParams io1, StGpioParams io2);