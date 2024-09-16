/**
 * 
 * 
 */

#pragma once

#include "gpio.h"

#include <stdint.h>
#include <stdbool.h>

#define ST_GPIO_MAX_PINS                        16

/**
  * @brief General Purpose I/O
  */
typedef struct
{
    volatile uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
    volatile uint32_t OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
    volatile uint32_t OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
    volatile uint32_t PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
    volatile uint32_t IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
    volatile uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
    volatile uint32_t BSRR;     /*!< GPIO port bit set/reset register,      Address offset: 0x18      */
    volatile uint32_t LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
    volatile uint32_t AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
} ST_GPIO_TypeDef;

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
} StGpioConfig;

typedef struct
{
    uint8_t pin_num;
    ST_GPIO_TypeDef * instance;
} StPrivGpio;

bool St_Gpio_Init(Gpio* gpio, StPrivGpio* st_gpio, uint32_t base_addr, uint8_t pin_num);
void St_Gpio_Config(Gpio* gpio, StGpioConfig* config);
bool St_Gpio_Toggle(Gpio* gpio);
void St_Gpio_Set(Gpio* gpio, bool active);
bool St_Gpio_Read(Gpio* gpio);
