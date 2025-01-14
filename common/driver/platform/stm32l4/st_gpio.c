
#include "st_gpio.h"

static inline void set_field(volatile uint32_t* field, uint32_t val,
                             uint32_t pos, uint8_t bits)
{
    uint32_t mask = (0x1 << bits) - 1;
    *field &= ~(mask << (pos * bits));

    /*
     * Mask to max value.
     */
    *field |= ((val & mask) << (pos * bits));
}

bool StGpioInit(Gpio* gpio, StGpioParams* params)
{
    if (params->pin_num >= ST_GPIO_MAX_PINS)
    {
        return false;
    }

    params->priv.instance = (GPIO_TypeDef *) params->base_addr;
    params->priv.pin_num = params->pin_num;
    params->priv.config = &params->conf;
    gpio->priv = (void *) &params->priv;
    gpio->config = StGpioConfig;
    gpio->toggle = StGpioToggle;
    gpio->set = StGpioSet;
    gpio->read = StGpioRead;

    return true;
}

void StGpioConfig(Gpio* gpio)
{
    StPrivGpio *dev = (StPrivGpio *) gpio->priv;

    /*
     * Pins >= 8 use AFR[1], pins < 8 use AFR[0].
     * For AF High, use index 'pin_num' - 8.
     */
    uint8_t afr_section = dev->pin_num / 8;
    uint8_t af_index = (afr_section == 1) ? dev->pin_num - 8 : dev->pin_num;

    set_field(&dev->instance->MODER, dev->config->mode, dev->pin_num, 2);
    set_field(&dev->instance->OTYPER, dev->config->otype, dev->pin_num, 1);
    set_field(&dev->instance->OSPEEDR, dev->config->ospeed, dev->pin_num, 2);
    set_field(&dev->instance->PUPDR, dev->config->pupd, dev->pin_num, 2);
    set_field(&dev->instance->AFR[afr_section], dev->config->af, af_index, 4);
}

static bool StGpioOut_Status(Gpio* gpio)
{
    StPrivGpio *dev = (StPrivGpio *) gpio->priv;

    return dev->instance->ODR & (0x1 << dev->pin_num);
}

bool StGpioToggle(Gpio* gpio)
{
    bool state = StGpioOut_Status(gpio);
    
    StGpioSet(gpio, !state);

    return !state;
}

void StGpioSet(Gpio* gpio, bool active)
{
    StPrivGpio *dev = (StPrivGpio *) gpio->priv;

    if (active)
    {
        dev->instance->ODR |= 0x1 << dev->pin_num;
    }
    else
    {
        dev->instance->ODR &= ~(0x1 << dev->pin_num);
    }
    
}

bool StGpioRead(Gpio* gpio)
{
    StPrivGpio *dev = (StPrivGpio *) gpio->priv;

    return dev->instance->IDR & (0x1 << dev->pin_num);
}