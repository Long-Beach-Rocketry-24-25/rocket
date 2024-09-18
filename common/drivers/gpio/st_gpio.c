
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

bool St_Gpio_Init(Gpio* gpio, StPrivGpio* st_gpio, uint32_t base_addr, uint8_t pin_num)
{
    if (pin_num >= ST_GPIO_MAX_PINS)
    {
        return false;
    }

    st_gpio->instance = (ST_GPIO_TypeDef *) base_addr;
    st_gpio->pin_num = pin_num;
    gpio->priv = (void *) st_gpio;
    gpio->toggle = St_Gpio_Toggle;
    gpio->set = St_Gpio_Set;
    gpio->read = St_Gpio_Read;

    return true;
}

void St_Gpio_Config(Gpio* gpio, StGpioConfig* config)
{
    StPrivGpio *dev = (StPrivGpio *) gpio->priv;

    /*
     * Pins >= 8 use AFR[1], pins < 8 use AFR[0].
     * For AF High, use index 'pin_num' - 8.
     */
    uint8_t afr_section = dev->pin_num / 8;
    uint8_t af_index = (afr_section == 1) ? dev->pin_num - 8 : dev->pin_num;

    set_field(&dev->instance->MODER, config->mode, dev->pin_num, 2);
    set_field(&dev->instance->OTYPER, config->otype, dev->pin_num, 1);
    set_field(&dev->instance->PUPDR, config->pupd, dev->pin_num, 2);
    set_field(&dev->instance->AFR[afr_section], config->af, af_index, 4);
}

static bool St_Gpio_Out_Status(Gpio* gpio)
{
    StPrivGpio *dev = (StPrivGpio *) gpio->priv;

    return dev->instance->ODR & (0x1 << dev->pin_num);
}

bool St_Gpio_Toggle(Gpio* gpio)
{
    bool state = St_Gpio_Out_Status(gpio);
    
    St_Gpio_Set(gpio, !state);

    return !state;
}

void St_Gpio_Set(Gpio* gpio, bool active)
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

bool St_Gpio_Read(Gpio* gpio)
{
    StPrivGpio *dev = (StPrivGpio *) gpio->priv;

    return dev->instance->IDR & (0x1 << dev->pin_num);
}