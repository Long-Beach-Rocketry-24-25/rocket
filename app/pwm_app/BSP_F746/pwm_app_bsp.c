

#include "pwm_app_bsp.h"

static StGpioParams led_stgpio = {{0}, GPIOB_BASE, 0, {ALT_FUNC, 0, 0, 0, 0x2}};

static StPrivPwm st_pwm;

void BSP_Init(Pwm* pwm_timer, Gpio* led_gpio)
{

    // LED GPIO
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    StGpioInit(led_gpio, &led_stgpio);
    StGpioConfig(led_gpio);

    StPwmInit(pwm_timer, &st_pwm, TIM3_BASE, 16000000, UINT16_MAX);
}