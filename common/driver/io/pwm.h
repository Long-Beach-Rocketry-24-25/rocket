/**
 * @brief PWM peripheral interface.
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief PWM interface instance.
 */
typedef struct Pwm Pwm;
struct Pwm
{
    /**
     * @brief Sets the desired PWM frequency to operate at.
     * 
     * @param pwm A pointer to the peripheral instance.
     * @param hz Desired frequency in Hz.
     * @return True on success, false otherwise.
     */
    bool (*set_freq)(Pwm* pwm, size_t hz);

    /**
     * @brief Sets the desired duty cycle for pwm device.
     * 
     * @param pwm A pointer to the peripheral instance.
     * @param duty Desired duty cycle given as a percentage of the period.
	 * @return True on success, false otherwise.
     */
    bool (*set_duty)(Pwm* pwm, double duty);

    /**
     * @brief A private context parameter for concrete implementations to use.
     */
    void* priv;
};
