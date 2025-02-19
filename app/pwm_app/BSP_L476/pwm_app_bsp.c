

#include "pwm_app_bsp.h"

static StGpioParams led_stgpio = {{0}, GPIOA_BASE, 5, {GPOUT, 0, 0, 0, 0}};

//adding paramters for user to manipulate
void BSP_Init(Pwm* pwm_timer, Gpio* led_gpio, size_t base_address, size_t mc_clock, bool enable, size_t hz, size_t duty){

    HAL_InitTick(0);
    SystemClock_Config();

    // LED GPIO
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    StGpioInit(led_gpio, &led_stgpio);
    StGpioConfig(led_gpio);


    StPwmInit(pwm_timer, pwm_timer, base_address, mc_clock);

    StPwmEnable(pwm_timer, enable);

    StPwmSetFreq(pwm_timer, hz);

    StPwmDuty(pwm_timer, duty);
}