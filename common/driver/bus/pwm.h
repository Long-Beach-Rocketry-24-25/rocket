#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


typedef  struct Pwm Pwm;

struct Pwm
{
    bool (*enable) (Pwm *pwm, bool enable);
    void (*setFreq) (Pwm *pwm, size_t hz);
    void (*setDuty) (Pwm *pwm, size_t duty); 
    void *priv;
};




