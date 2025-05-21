/**
 * A motor control interface for any continuously driveable
 * motor in one or both directions.
 */

#pragma once

#include "gpio.h"
#include "pwm.h"

#include <stdbool.h>

/**
 * Motor Control Directions - Clockwise and Counter Clockwise.
 */
typedef enum
{
    CW,
    CCW
} MotorDirection;

/**
 * A motor instance.
 */
typedef struct Motor Motor;
struct Motor
{

    /**
     * Enables/disables the motor to be driven. When disabled, the motor
     * should be stopped regardless of direction or power settings.
     * 
     * @param motor the motor interface instance.
     * @param enable true to enable, false to disable.
     * @return True on success, false otherwise. 
     */
    bool (*set_enabled)(Motor* motor, bool enable);

    /**
     * Sets the direction for the motor to rotate when driven.
     * 
     * @param motor the motor interface instance.
     * @param direction the direction to rotate when driven.
     * @return true on success, false otherwise.
     */
    bool (*set_direction)(Motor* motor, MotorDirection direction);

    /**
     * Sets the power percentage to drive the motor, from 0-100%.
     * This should immediately take effect if the motor is enabled.
     * 
     * @param motor the motor interface instance.
     * @param percentage the percentage to power the motor.
     * @return true on success, false otherwise.
     */
    bool (*set_power)(Motor* motor, float percentage);

    /**
     * Private context for implementations of the motor.
     */
    void* priv;
};