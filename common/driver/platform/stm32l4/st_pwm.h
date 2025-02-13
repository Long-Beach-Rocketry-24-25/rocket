

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
    TIM_TypeDef * instance;
    Pwm enable;
    Pwm freq;
    Pwm duty;
    
    
};  StPrivPwm;

bool Pwm_enable(Pwm *pwm, bool enable);
void Pwm_set_frequency(Pwm *pwm, size_t freq);
void Pwm_set_duty(Pwm *pwm, size_t duty);
