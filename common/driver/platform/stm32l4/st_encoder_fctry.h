
#pragma once

#include "alloc.h"
#include "exit.h"
#include "st_encoder.h"
#include "st_gpio_fctry.h"

bool GiveStEncoder(Encoder* enc, Mem* mem, const uint32_t base_addr,
                   const StEncoderCmpChannel channel_polarity,
                   const StGpioParams io1, const StGpioParams io2);

Encoder* MakeStEncoder(Mem* mem, const uint32_t base_addr,
                       const StEncoderCmpChannel channel_polarity,
                       const StGpioParams io1, const StGpioParams io2);