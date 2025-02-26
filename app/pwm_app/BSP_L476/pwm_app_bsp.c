

#include "pwm_app_bsp.h"

static StGpioParams led_stgpio = {{0}, GPIOA_BASE, 5, {ALT_FUNC, 0, 0, 0, 0x1}};

static StPrivPwm st_pwm;
static size_t base_address = TIM2_BASE;
static size_t mc_clock = 84000000;

void BSP_Init(Pwm* pwm_timer, Gpio* led_gpio)
{

    HAL_InitTick(0);

    //SystemClock_Config();

    // LED GPIO
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
    GPIOA->MODER |= GPIO_MODER_MODE5_1;
    GPIOA->AFR[0] |= GPIO_AFRL_AFSEL1_0;
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5_Msk;

    StGpioInit(led_gpio, &led_stgpio);
    StGpioConfig(led_gpio);

    StPwmInit(pwm_timer, &st_pwm, base_address, mc_clock);
}