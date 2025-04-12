#include "fake_dcm.h"

void Drv8242Init(DCMotor* motor, DCPosControl* control, Pwm* pwm)
{
    motor->set_en = Drv8242SetEnable;
    motor->set_duty = Drv8242SetDuty;
    motor->set_direction = Drv8242SetDir;
    motor->priv = (void*)control;

    control->state = OFF;
    control->enabled = 1;
    control->disabled = 0;
    control->direction_one = 0;
    control->direction_two = 1;
    control->duty = 50;

    control->pwm = pwm;

    Drv8242SetEnable(motor, false);
}

bool Drv8242SetEnable(DCMotor* motor, bool enable)
{
    DCPosControl* dev = (DCPosControl*)motor->priv;

    if (enable)
    {
        dev->state = START;
        return dev->pwm->enable(dev->pwm, true);
    }
    else
    {
        dev->state = OFF;
        return dev->pwm->enable(dev->pwm, false);
    }
}

bool Drv8242etDir(DCMotor* motor, bool direction)
{
    DCPosControl* dev = (DCPosControl*)motor->priv;

    if (motor->set_en)
    {
        if (direction)
        {
            dev->state = RUNNING;
            return true;
        }
        else
        {
            dev->state = RUNNING;
            return true;
        }
    }
    else
    {
        return false;
    }
}

void Drv8242SetDuty(DCMotor* motor, float duty)
{
    DCPosControl* dev = (DCPosControl*)motor->priv;
    dev->pwm->setDuty(dev->pwm, duty);
}
