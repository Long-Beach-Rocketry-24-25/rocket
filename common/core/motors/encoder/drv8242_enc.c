#include "drv8242_enc.h"

void QEnc_Init(QEnc* qenc, DCEncParams* params)
{
    qenc->getTicks = get_ticks;
    qenc->priv = (void*)params;

    params->counter = 0;
    params->tick_size = 0;
    params->ticks_per_angle = 0;
}

bool get_ticks(QEnc* qenc, size_t angle, size_t tick_size, size_t ppr)
{
    DCEncParams* params = (DCEncParams*)(qenc->priv);
    params->tick_size = tick_size;
    params->ticks_per_angle = (int)(ppr / 90);
    params->angle = angle;
}

bool set_ticks(QEnc* qenc)
{
    DCEncParams* params = (DCEncParams*)(qenc->priv);
    params->ticks_needed = (params->ticks_per_angle) * (params->angle);
}