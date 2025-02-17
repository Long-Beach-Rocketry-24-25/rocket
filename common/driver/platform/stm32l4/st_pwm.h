

#pragma once

#include "stm32l4xx.h"

#include "gpio.h"
#include "pwm.h"
#include "timeout.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


typedef struct
{
    TIM_TypeDef *instance;

    size_t period;
    size_t clock;
    
    
}  StPrivPwm;

void StPwmInit(Pwm *pwm, StPrivPwm* st_pwm, size_t base_address, size_t mc_clock);
bool StPwmEnable(Pwm *pwm, bool enable);
void StPwmSetFreq(Pwm *pwm, size_t hz);
void StPwmDuty(Pwm *pwm, size_t duty);
