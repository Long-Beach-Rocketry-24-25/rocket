
#include "nidec_24h.h"

bool give_nidec_24h(Motor* motor, Mem* mem, Pwm* pwm, Gpio* brake,
                    Gpio* direction)
{
    Nidec24H* nidec = ALLOC(mem, Nidec24H);
    EXIT_IF(nidec == NULL, false);
    EXIT_IF(!Nidec24HInit(motor, nidec, pwm, brake, direction), false);
    return true;
}

Motor* make_nidec_24h(Mem* mem, Pwm* pwm, Gpio* brake, Gpio* direction)
{
    Motor* motor = ALLOC(mem, Motor);
    EXIT_IF(motor == NULL, NULL);
    EXIT_IF(!give_nidec_24h(motor, mem, pwm, brake, direction), NULL);
    return motor;
}

bool Nidec24HInit(Motor* motor, Nidec24H* nidec, Pwm* pwm, Gpio* brake,
                  Gpio* direction)
{
    /**
     * One-time configuration of PWM frequency to Nidec 24H default.
     */
    if (!pwm->set_freq(pwm, NIDEC_24H_PWM_FREQ_HZ))
    {
        return false;
    }

    nidec->pwm = pwm;
    nidec->brake = brake;
    nidec->direction = direction;

    motor->priv = (void*)nidec;
    motor->set_enabled = Nidec24HSetEnable;
    motor->set_direction = Nidec24HSetDirection;
    motor->set_power = Nidec24HSetPower;

    return true;
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

    if (percentage < 0 || percentage > 100)
    {
        return false;
    }

    /**
     * Map from 0 - 100% input range to motor min - motor max range.
     * An input percentage of 0 should still, however correlate to 0.
     */
    float mapped =
        (percentage == 0)
            ? 0
            : ((NIDEC_24H_PWM_MAX - NIDEC_24H_PWM_MIN) * percentage / 100) +
                  NIDEC_24H_PWM_MIN;

    /** 
     * Power control reflects reverse of PWM duty cycle,
     * for example, a MOTOR power drive percentage of 80%
     * needs a PWM signal at 20% duty cycle.
     */
    return dev->pwm->set_duty(dev->pwm, 100.0 - mapped);
}
