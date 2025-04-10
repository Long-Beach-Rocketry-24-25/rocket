
#pragma once

#include "drv8242_dcm.h"
#include "gpio.h"
#include "pwm.h"
#include "stm32l476xx.h"

typedef struct DCM_Control DCM_Control;

struct DCM_Control
{
    bool cmd_enable;
    size_t pusle_per_rev;
};

void DCM_Control_Init(Pwm* pwm_timer, DCMotor* motor, Gpio* nSleep_pwm,
                      Gpio* drvoff_pin, Gpio* en_in1_pin, Gpio* ph_in2_pin);

void config(DCM_Control* dc_control, size_t pulses_per_rev);

void update();

int get_state();
