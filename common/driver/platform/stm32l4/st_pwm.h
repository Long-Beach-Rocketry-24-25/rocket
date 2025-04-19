/**
 * @brief STM32L4 implementation of PWM interface.
 */

#pragma once

#include "stm32l4xx.h"

#include "pwm.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct
{
    TIM_TypeDef* instance;
    size_t period;
    size_t clock;
} StPrivPwm;

/**
  * @brief Initializes settings for a PWM object.
  * 
  * @param pwm A pointer to the peripheral instance.
  * @param st_pwm A pointer to a private data variable giving access to other private members.
  * @param base_address The base address of the PWM peripheral.
  * @param mc_clock This represents the Micro-controller's native PWM clock frequency.
  * 
  */
void StPwmInit(Pwm* pwm, StPrivPwm* st_pwm, size_t base_address,
               size_t mc_clock);

/**
  * @brief Initializes settings for a PWM object.
  * 
  * @param pwm A pointer to the peripheral instance.
  */
void StPwmConfig(Pwm* pwm);

/**
  * @brief Sets the desired pwm frequency to operate at.
  * 
  * @param pwm A pointer to the peripheral instance.
  * @param hz Desired frequency in Hz set by user.
  * 
  */
bool StPwmSetFreq(Pwm* pwm, size_t hz);

/**
  * @brief Sets the desired duty cycle for pwm device.
  * 
  * @param pwm A pointer to the peripheral instance.
  * @param duty Desired duty cycle given as a percentage of the period (1000ms / Freq in Hz). 
  * 
  */
bool StPwmDuty(Pwm* pwm, double duty);
