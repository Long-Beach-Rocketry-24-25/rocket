

#include "pwm_app_bsp.h"

static StPrivPwm st_pwm;
static StPrivUsart st_usart;
// static StGpioParams led_stgpio = {{0}, GPIOB_BASE, 0, {ALT_FUNC, 0, 0, 0, 0x2}};
static StGpioParams pwm_stgpio = {{0}, GPIOA_BASE, 3, {ALT_FUNC, 0, 0, 0, 0x1}};

// Sequential use of these, so using one is fine. Not thread safe.
static Timeout time;
static FrtTimerData frt;

static StGpioParams uart_io1 = {{0},
                                GPIOD_BASE,
                                8,
                                {ALT_FUNC, 0, 0, 0, 0x7}};  // USART3 AF 7
static StGpioParams uart_io2 = {{0},
                                GPIOD_BASE,
                                9,
                                {ALT_FUNC, 0, 0, 0, 0x7}};  // USART3 AF 7

void BSP_Init(Usart* usart, Pwm* pwm_timer, Gpio* led_gpio)
{

    // LED GPIO
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    StGpioInit(led_gpio, &pwm_stgpio);
    StGpioConfig(led_gpio);

    //TIM2 CH4
    StPwmInit(pwm_timer, &st_pwm, TIM2_BASE, 4, 16000000, UINT16_MAX);
    StPwmConfig(pwm_timer);

    // Single FreeRTOS timer
    frt_timer_init(&time, &frt, 100);

    // USART3
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

    StGpioInit(&st_usart.rx, &uart_io1);
    StGpioInit(&st_usart.tx, &uart_io2);

    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(0, 6, 0));
    NVIC_EnableIRQ(USART3_IRQn);

    StUsartInit(usart, &st_usart, USART3_BASE, &time);
    StUsartConfig(usart, SystemCoreClock, 115200);
}

void USART3_IRQHandler(void)
{
    cli_usart_rx_callback();
}