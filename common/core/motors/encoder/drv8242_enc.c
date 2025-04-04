#include "drv8242_enc.h"
#include <math.h>

void QEnc_Init(QEnc* qenc, MotorRotoationCtrler* params)
{
    qenc->getTicks = get_ticks;
    qenc->priv = (void*)params;

    params->counter = 0;
    params->ticks_per_angle = 0;
    params->command_rotate = command_rotate;
    params->update = update;
    params->cmd = false;
    params->ppr = 100;
}

size_t get_ticks(QEnc* qenc)
{
    MotorRotoationCtrler* params = (MotorRotoationCtrler*)(qenc->priv);
    return params->counter;
}

size_t increment(QEnc* qenc)
{
    MotorRotoationCtrler* controller = (MotorRotoationCtrler*)(qenc->priv);
    if (fabs(controller->diff) > 65535)
    {
        controller->counter = 0;
        return controller->counter;
    }
    else
    {
        return ++controller->counter;
    }
}

size_t decrement(QEnc* qenc)
{
    MotorRotoationCtrler* controller = (MotorRotoationCtrler*)(qenc->priv);
    if (fabs(controller->diff) > 65535)
    {
        controller->counter = 0;
        return controller->counter;
    }
    else
    {
        return --controller->counter;
    }
}

bool command_rotate(MotorRotoationCtrler* controller, double degrees)
{
    if (!controller->cmd)
    {
        return false;
    }
    else
    {
        controller->ticks_per_angle = (int)(controller->ppr / 90);
        controller->ticks_needed = (controller->ticks_per_angle) * (degrees);
        controller->dir = degrees > 0;
        controller->start_pos =
            controller->encoder->getTicks(controller->encoder);
        controller->cmd = false;
        return true;
    }
}

bool update(MotorRotoationCtrler* controller)
{
    size_t curr_enc = controller->encoder->getTicks(controller->encoder);
    switch (controller->state)
    {
        case idle:
            if (controller->cmd)
            {
                controller->motor->set_direction(controller->motor,
                                                 controller->dir);
                controller->motor->set_en(controller->motor, true);
                controller->state = rotating;
            }
            break;
        case rotating:
            controller->diff = curr_enc - controller->last_enc;
            if (fabs(controller->diff) < controller->ticks_per_angle)
            {
                controller->motor->set_en(controller->motor, false);
                controller->state = idle;
                controller->cmd = true;
            }
            break;
    }
    controller->last_enc = curr_enc;
}