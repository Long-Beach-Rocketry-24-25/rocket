#include "drv8242_dcm.h"

void Drv8242Init(DCMotor* motor, DCPosControl* control, Gpio* nSleep_pin,
                 Gpio* drvoff_pin, Gpio* en_in1_pin, Gpio* ph_in2_pin, Pwm* pwm)
{
    motor->set_en = Drv8242SetEnable;
    motor->set_duty = Drv8242SetDuty;
    motor->set_direction = Drv8242SetDir;
    motor->priv = (void*)control;

    control->nSleep_pin = nSleep_pin;
    control->drvoff_pin = drvoff_pin;
    control->en_in1_pin = en_in1_pin;
    control->ph_in2_pin = ph_in2_pin;

    control->state = OFF;

    control->pwm = pwm;

    Drv8242SetEnable(motor, false);
}

bool Drv8242SetEnable(DCMotor* motor, bool enable)
{
    DCPosControl* dev = (DCPosControl*)motor->priv;

    if (enable)
    {
        dev->nSleep_pin->set(dev->nSleep_pin, true);
        dev->drvoff_pin->set(dev->drvoff_pin, false);  //setting to idle
        dev->en_in1_pin->set(dev->en_in1_pin, false);
        dev->ph_in2_pin->set(dev->ph_in2_pin, false);
        dev->state = START;
        return dev->pwm->enable(dev->pwm, true);
    }
    else
    {
        dev->nSleep_pin->set(dev->nSleep_pin, false);
        dev->state = OFF;
        return dev->pwm->enable(dev->pwm, false);
    }
}

bool Drv8242etDir(DCMotor* motor, bool direction)
{
    DCPosControl* dev = (DCPosControl*)motor->priv;

    if (Drv8242SetEnable)
    {
        if (direction)
        {
            dev->en_in1_pin->set(dev->en_in1_pin, true);
            dev->ph_in2_pin->set(dev->ph_in2_pin, false);
            dev->state = RUNNING;
            return true;
        }
        else
        {
            dev->en_in1_pin->set(dev->en_in1_pin, true);
            dev->ph_in2_pin->set(dev->ph_in2_pin, true);
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
