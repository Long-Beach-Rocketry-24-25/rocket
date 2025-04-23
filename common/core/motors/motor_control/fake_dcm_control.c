#include "fake_dcm_control.h"

void DCM_Control_Init(DCM_Control* control, DCPosControl* motor,
                      MotorRotoationCtrler* qenc)
{
    control->motor = motor;
    control->qenc = qenc;
    control->cmd_enable = false;
}

void config(DCM_Control* control)
{
    switch (control->motor->state)
    {
        case START:
            if (!control->qenc->cmd)
            {
                control->qenc->cmd = true;
            }
            break;

        default:
            break;
    }
}

void motor_update(DCM_Control* control)
{
    switch (control->qenc->state)
    {
        case idle:
            control->qenc->update(control->qenc);
            control->cmd_enable = true;
            break;

        case rotating:
            control->qenc->update(control->qenc);
            control->cmd_enable = false;
            break;
    }
}

int get_state(DCM_Control* control)
{
    return control->motor->state;
}