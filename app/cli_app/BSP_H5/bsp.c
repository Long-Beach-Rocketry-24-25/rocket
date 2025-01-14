
#include "bsp.h"

static StPrivUsart st_usart;
static StPrivI2c st_i2c;
static StGpioParams led_stgpio = {{ 0 }, GPIOB_BASE, 0, {1, 0, 0, 0, 0}};

// Sequential use of these, so using one is fine. Not thread safe.
static Timeout time;
static FrtTimerData frt;

static StGpioParams uart_io1 = {{ 0 }, GPIOD_BASE, 8, 
                                {ALT_FUNC, 0, 0, 0, 0x7}}; // USART3 AF 7
static StGpioParams uart_io2 = {{ 0 }, GPIOD_BASE, 9,
                                {ALT_FUNC, 0, 0, 0, 0x7}}; // USART3 AF 7

const StGpioSettings i2c_io_conf = {ALT_FUNC, OPEN_DRAIN, 0, PULL_UP, 0x4};

static StGpioParams i2c1_io1 = {{ 0 }, GPIOC_BASE, 8, i2c_io_conf};
static StGpioParams i2c1_io2 = {{ 0 }, GPIOC_BASE, 9, i2c_io_conf};

void BSP_Init(Usart *usart, I2c *temp_i2c, Gpio *led_gpio)
{
    // LED GPIO
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

    StGpioInit(led_gpio, &led_stgpio);
    StGpioConfig(led_gpio);

    // Single FreeRTOS timer
    frt_timer_init(&time, &frt, 100);

    // USART3
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;

    StGpioInit(&st_usart.rx, &uart_io1);
    StGpioInit(&st_usart.tx, &uart_io2);

    RCC->APB1LENR |= RCC_APB1LENR_USART3EN;

    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(0, 6, 0));
    NVIC_EnableIRQ(USART3_IRQn);

    StUsartInit(usart, &st_usart, USART3_BASE, &time);
    StUsartConfig(usart, SystemCoreClock, 115200);

    // I2c1 PC8, PC9
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

    StGpioInit(&st_i2c.scl, &i2c1_io1);
    StGpioInit(&st_i2c.sda, &i2c1_io2);

    RCC->APB1LENR |= RCC_APB1LENR_I2C1EN;

    StI2cInit(temp_i2c, &st_i2c, I2C1_BASE, &time);
    StI2cConfig(temp_i2c, 0x60808CD3);
}
