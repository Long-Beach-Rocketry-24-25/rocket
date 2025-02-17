/**
 * @brief this file sets up the PWM drivers for use with various timers
 */


#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "stm32l4xx.h"

/**
 * @brief structure that contains the functions needed to intialize and setup specific timers usint PWM drivers.
 */
typedef struct Pwm Pwm;

struct Pwm
{
/**
 * @brief enables the timers and their registers to setup PWM
 * 
 * @param pwm a pointer to the peripheral instance.
 * @param enable true to enable the PWM peripheral, false to disable.
 * 
 * @return true on success, false otherwise.
 */
    bool (*enable) (Pwm *pwm, bool enable);

/**
 * @brief sets the desired pwm frequency to operate at
 * 
 * @param pwm a pointer to the peripheral instance.
 * @param hz desired frequency in Hz set by user
 * 
 */
    void (*setFreq) (Pwm *pwm, size_t hz);

/**
 * @brief sets the desired duty cycle for pwm device
 * 
 * @param pwm a pointer to the peripheral instance.
 * @param duty desired duty cycle given as a percentage of the period.
 * 
 */
    void (*setDuty) (Pwm *pwm, size_t duty); 

    void *priv;
};




