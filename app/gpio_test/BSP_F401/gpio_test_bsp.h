/**
 * 
 */

#pragma once

#include <stdint.h>
#include "st_gpio.h"

void BSP_Init(Gpio* led_gpio);

typedef enum
{
    BUTTON_RELEASED = 0,
    BUTTON_PRESSED
} DebounceState;

typedef struct
{
    Gpio gpio;
    DebounceState state;
    uint8_t raw_state;
    uint32_t last_change_ms;
    uint32_t debounce_time_ms;
} DebounceButton_t;

void Debounce_Init(DebounceButton_t* btn, StGpioParams* gpio_params,
                   uint32_t debounce_time_ms);
void Debounce_Update(DebounceButton_t* btn, uint32_t now_ms);
uint8_t Debounce_IsPressed(DebounceButton_t* btn);
uint8_t Debounce_IsReleased(DebounceButton_t* btn);
DebounceState Debounce_GetState(DebounceButton_t* btn);
