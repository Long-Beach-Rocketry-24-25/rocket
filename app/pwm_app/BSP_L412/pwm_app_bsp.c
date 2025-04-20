

#include "pwm_app_bsp.h"

static StPrivPwm st_pwm;
static StPrivUsart st_usart;
static StGpioParams pwm_stgpio = {{0}, GPIOB_BASE, 3, {ALT_FUNC, 0, 0, 0, 0x1}};

// Sequential use of these, so using one is fine. Not thread safe.
static Timeout time;
static FrtTimerData frt;

static StGpioParams uart_io1 = {{0},
                                GPIOA_BASE,
                                2,
                                {ALT_FUNC, 0, 0, 0, 0x7}};  // USART2 AF 7
static StGpioParams uart_io2 = {{0},
                                GPIOA_BASE,
                                15,
                                {ALT_FUNC, 0, 0, 0, 0x3}};  // USART2 AF 3

void BSP_Init(Usart* usart, Pwm* pwm, Gpio* led_gpio)
{
    // LED GPIO
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

    StGpioInit(led_gpio, &pwm_stgpio);
    StGpioConfig(led_gpio);

    //TIM2 CH2
    StPwmInit(pwm, &st_pwm, TIM2_BASE, 2, 16000000, UINT16_MAX);
    StPwmConfig(pwm);

    // Single FreeRTOS timer
    frt_timer_init(&time, &frt, 100);

    // USART2
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    StGpioInit(&st_usart.rx, &uart_io1);
    StGpioInit(&st_usart.tx, &uart_io2);

    RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;

    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(0, 6, 0));
    NVIC_EnableIRQ(USART2_IRQn);

    StUsartInit(usart, &st_usart, USART2_BASE, &time);
    StUsartConfig(usart, SystemCoreClock, 115200);
}

void USART2_IRQHandler(void)
{
    cli_usart_rx_callback();
}