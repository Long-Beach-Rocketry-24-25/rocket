
#include "st_encoder_fctry.h"

bool GiveStEncoder(Encoder* enc, Mem* mem, const uint32_t base_addr,
                   const size_t channel_pair,
                   const StEncoderCmpChannel channel_polarity,
                   const StGpioParams io1, const StGpioParams io2)
{
    StPrivEncoder* st = ALLOC(mem, StPrivEncoder);
    EXIT_IF(st == NULL, false);

    EXIT_IF(MakeStGpio(mem, io1) == NULL, false);
    EXIT_IF(MakeStGpio(mem, io2) == NULL, false);

    return StEncoderInit(enc, st, base_addr, channel_pair, channel_polarity);
}

Encoder* MakeStEncoder(Mem* mem, const uint32_t base_addr,
                       const size_t channel_pair,
                       const StEncoderCmpChannel channel_polarity,
                       const StGpioParams io1, const StGpioParams io2)
{
    Encoder* encoder = ALLOC(mem, Encoder);
    EXIT_IF(encoder == NULL, NULL);
    EXIT_IF(!GiveStEncoder(encoder, mem, base_addr, channel_pair,
                           channel_polarity, io1, io2),
            NULL);
    return encoder;
}