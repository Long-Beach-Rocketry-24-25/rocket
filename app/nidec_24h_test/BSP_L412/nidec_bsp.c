

#include "nidec_bsp.h"

#define EXIT_IF_FAIL(cond) EXIT_IF(!(cond), false)

Mem memory;
static uint8_t driver_mem[DRIVER_MEM_SIZE] = {0};

bool BSP_Init(Usart* usart, Motor* motor, Encoder* encoder)
{
    EXIT_IF_FAIL(InitPrealloc(&memory, driver_mem, DRIVER_MEM_SIZE));

    // Single FRT timer.
    Timeout* time = make_frt_timer(&memory, 100);
    EXIT_IF(time == NULL, false);

    // USART2
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;

    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(0, 6, 0));
    NVIC_EnableIRQ(USART2_IRQn);

    // PA2 AF7/ PA15 AF3
    EXIT_IF_FAIL(GiveStUsart(
        usart, &memory, time, USART2_BASE, SystemCoreClock, 115200,
        (StGpioParams){{0}, GPIOA_BASE, 2, {ALT_FUNC, 0, 0, 0, 0x7}},
        (StGpioParams){{0}, GPIOA_BASE, 15, {ALT_FUNC, 0, 0, 0, 0x3}}));

    /**
     * TIM2 CH1 (PA0 AF1):
     * With no additional clock config, APB1 clock should be same frequency
     * as core clock.
     */
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
    EXIT_IF_FAIL(give_nidec_24h(
        motor, &memory,
        MakeStPwm(&memory, TIM2_BASE, 1, HAL_RCC_GetPCLK1Freq(), UINT16_MAX,
                  (StGpioParams){{0}, GPIOA_BASE, 0, {ALT_FUNC, 0, 0, 0, 0x1}}),
        MakeStGpio(&memory,
                   (StGpioParams){{0}, GPIOA_BASE, 1, {GPOUT, 0, 0, 0, 0}}),
        MakeStGpio(&memory,
                   (StGpioParams){{0}, GPIOA_BASE, 3, {GPOUT, 0, 0, 0, 0}})));

    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    EXIT_IF_FAIL(GiveStEncoder(
        encoder, &memory, TIM1_BASE, ONE_TO_ONE,
        (StGpioParams){{0}, GPIOA_BASE, 8, {ALT_FUNC, 0, 0, 0, 1}},
        (StGpioParams){{0}, GPIOA_BASE, 9, {ALT_FUNC, 0, 0, 0, 1}}));

    return true;
}

void USART2_IRQHandler(void)
{
    cli_usart_rx_callback();
}