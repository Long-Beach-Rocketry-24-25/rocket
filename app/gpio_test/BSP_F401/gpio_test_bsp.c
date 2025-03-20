
#include "gpio_test_bsp.h"

static StGpioParams led_stgpio = {{0}, GPIOA_BASE, 5, {GPOUT, 0, 0, 0, 0}};

void BSP_Init(Gpio* led_gpio)
{

    // LED GPIO
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    StGpioInit(led_gpio, &led_stgpio);
    StGpioConfig(led_gpio);

    // Single FreeRTOS timer
    // frt_timer_init(&time, &frt, 100);

    // RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;

    // StUsartInit(usart, &st_usart, USART2_BASE, &time);
    // StUsartConfig(usart, SystemCoreClock, 115200);

    // // I2c1 PB8, PB9
    // RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

    // RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;

    // StI2cInit(temp_i2c, &st_i2c1, I2C1_BASE, &time);
    // StI2cConfig(temp_i2c, 0x10909CEC);

    // void Debounce_Init(DebounceButton_t *btn, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint32_t debounce_time_ms)
}
