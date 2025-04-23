
#include "nidec_24h.h"

bool Nidec24HInit(Motor* motor, Nidec24H* nidec, Pwm* pwm, Gpio* brake,
                  Gpio* direction)
{
    if (!pwm->set_freq(pwm, NIDEC_24H_PWM_FREQ_HZ))
    {
        return false;
    }

    nidec->pwm = pwm;
    nidec->brake = brake;
    nidec->direction = direction;

    motor->priv = (void*)nidec;
    motor->set_en = Nidec24HSetEnable;
    motor->set_direction = Nidec24HSetDirection;
    motor->set_power = Nidec24HSetPower;
}

bool Nidec24HSetEnable(Motor* motor, bool enable)
{
    Nidec24H* dev = (Nidec24H*)motor->priv;

    // Brake off = high or floating, Brake on = low.
    dev->brake->set(dev->brake, enable);
    return true;
}

bool Nidec24HSetDirection(Motor* motor, MotorDirection direction)
{
    Nidec24H* dev = (Nidec24H*)motor->priv;

    // CW = high or floating, CCW = low.
    dev->direction->set(dev->direction, (direction == CW) ? true : false);
    return true;
}

bool Nidec24HSetPower(Motor* motor, float percentage)
{
    Nidec24H* dev = (Nidec24H*)motor->priv;

    if (percentage < NIDEC_24H_PWM_MIN || percentage > NIDEC_24H_PWM_MAX)
    {
        return false;
    }

    /** 
     * Power control reflects reverse of PWM duty cycle,
     * for example, a MOTOR power drive percentage of 80%
     * needs a PWM signal at 20% duty cycle.
     */
    dev->pwm->set_duty(dev->pwm, 100.0 - percentage);
    return true;
}
