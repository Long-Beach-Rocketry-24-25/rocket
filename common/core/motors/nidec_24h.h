/**
 * 
 */

#pragma once

#include "gpio.h"
#include "pwm.h"

#include "alloc.h"
#include "exit.h"
#include "motor.h"

/**
 * Nidec 24H is capable of frequencies between 15KHz to 25KHz.
 */
#define NIDEC_24H_PWM_FREQ_HZ (20000)
#define NIDEC_24H_PWM_MIN (15.0)
#define NIDEC_24H_PWM_MAX (100.0)

typedef struct
{
    Pwm* pwm;
    Gpio* brake;
    Gpio* direction;
} Nidec24H;

/**
 * 
 * 
 */
bool give_nidec_24h(Motor* motor, Mem* mem, Pwm* pwm, Gpio* brake,
                    Gpio* direction);

/**
 * 
 */
Motor* make_nidec_24h(Mem* mem, Pwm* pwm, Gpio* brake, Gpio* direction);

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