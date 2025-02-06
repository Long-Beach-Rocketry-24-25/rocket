/**
 * 
 * 
 */

#pragma once

#include "stm32h5xx.h"

#include "gpio.h"

#include <stdbool.h>
#include <stdint.h>

#define ST_GPIO_MAX_PINS 16

typedef enum
{
    INPUT = 0,
    GPOUT,
    ALT_FUNC,
    ANALOG,
} StGpioMode;

typedef enum
{
    PUSH_PULL = 0,
    OPEN_DRAIN
} StGpioOtype;

typedef enum
{
    LOW = 0,
    MEDIUM,
    HIGH,
    VERY_HIGH
} StGpioOspeed;

typedef enum
{
    NO_PULL = 0,
    PULL_UP,
    PULL_DOWN
} StGpioPupd;

typedef struct
{
    StGpioMode mode;
    StGpioOtype otype;
    StGpioOspeed ospeed;
    StGpioPupd pupd;
    uint8_t af;
} StGpioSettings;

typedef struct
{
    uint8_t pin_num;
    StGpioSettings* config;
    GPIO_TypeDef* instance;
} StPrivGpio;

typedef struct
{
    StPrivGpio priv;
    uint32_t base_addr;
    uint8_t pin_num;
    StGpioSettings conf;
} StGpioParams;

bool StGpioInit(Gpio* gpio, StGpioParams* params);
void StGpioConfig(Gpio* gpio);
bool StGpioToggle(Gpio* gpio);
void StGpioSet(Gpio* gpio, bool active);
bool StGpioRead(Gpio* gpio);
