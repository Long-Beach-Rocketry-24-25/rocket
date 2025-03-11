
#include "cli_app_bsp.h"

#define EXIT_IF_FAIL(cond) EXIT_IF(!(cond), false)

// Sequential use of these, so using one is fine. Not thread safe.
static Timeout time;
static FrtTimerData frt;
static Mem memory;
static uint8_t driver_mem[DRIVER_MEM_SIZE] = {0};

bool BSP_Init(Usart* usart, I2c* temp_i2c, Gpio* led_gpio)
{
    EXIT_IF_FAIL(InitStalloc(&memory, driver_mem, DRIVER_MEM_SIZE));

    // LED GPIO
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    EXIT_IF_FAIL(
        MakeStGpio(led_gpio, &memory,
                   (StGpioParams){{0}, GPIOB_BASE, 0, {1, 0, 0, 0, 0}}));

    // Single FreeRTOS timer
    frt_timer_init(&time, &frt, 100);

    // USART3
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(0, 6, 0));
    NVIC_EnableIRQ(USART3_IRQn);

    // PD8/9 AF 7
    EXIT_IF_FAIL(MakeStUsart(
        usart, &memory, USART3_BASE, SystemCoreClock, 115200, &time,
        (StGpioParams){{0}, GPIOD_BASE, 8, {ALT_FUNC, 0, 0, 0, 0x7}},
        (StGpioParams){{0}, GPIOD_BASE, 9, {ALT_FUNC, 0, 0, 0, 0x7}}));

    // I2C1
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    // PB8/9 AF 4
    const StGpioSettings i2c_conf = {ALT_FUNC, OPEN_DRAIN, 0, PULL_UP, 0x4};
    EXIT_IF_FAIL(MakeStI2c(temp_i2c, &memory, I2C1_BASE, 0x20B, &time,
                           (StGpioParams){{0}, GPIOB_BASE, 8, i2c_conf},
                           (StGpioParams){{0}, GPIOB_BASE, 9, i2c_conf}));

    return true;
}

void USART3_IRQHandler(void)
{
    cli_usart_rx_callback();
}