/**
 * 
 */

#pragma once

#include "alloc.h"

#include "gpio.h"
#include "pwm.h"

#include "motor.h"

#define NIDEC_24H_PWM_FREQ_HZ (20000)
#define NIDEC_24H_PWM_MIN (15.0)
#define NIDEC_24H_PWM_MAX (100.0)

typedef struct
{
    Pwm* pwm;
    Gpio* brake;
    Gpio* direction;
} Nidec24H;

Motor* make_nidec_24h(Memory* mem, Pwm* pwm, Gpio* brake, Gpio* direction);

/**
 * 
 */
bool Nidec24HInit(Motor* motor, Nidec24H* nidec, Pwm* pwm, Gpio* brake,
                  Gpio* direction);

/**
 * 
 */
bool Nidec24HSetEnable(Motor* motor, bool enable);

/**
 * 
 */
bool Nidec24HSetDirection(Motor* motor, MotorDirection direction);

/**
 * 
 */
bool Nidec24HSetPower(Motor* motor, float percentage);