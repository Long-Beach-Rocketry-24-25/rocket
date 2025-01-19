
#include "subscale_bsp.h"

static StPrivUsart st_usart;
static StPrivSpi st_spi;
static GpioChipSelect st_spi_cs;
static StPrivI2c st_i2c;
static StGpioParams led_stgpio = {{ 0 }, GPIOB_BASE, 0, {1, 0, 0, 0, 0}};
static StGpioParams spi_cs_io = {{ 0 }, GPIOD_BASE, 14, {1, 0, 0, 0, 0}};

// Sequential use of these, so using one is fine. Not thread safe.
static Timeout time;
static FrtTimerData frt;

static StGpioParams uart_io1 = {{ 0 }, GPIOD_BASE, 8, 
                                {ALT_FUNC, 0, 0, 0, 0x7}}; // USART3 AF 7
static StGpioParams uart_io2 = {{ 0 }, GPIOD_BASE, 9,
                                {ALT_FUNC, 0, 0, 0, 0x7}}; // USART3 AF 7

const StGpioSettings spi_io_conf = {ALT_FUNC, 0, 0, 0, 0x5}; // SPI1 AF 5
static StGpioParams spi_io1 = {{ 0 }, GPIOA_BASE, 5, spi_io_conf};
static StGpioParams spi_io2 = {{ 0 }, GPIOA_BASE, 6, spi_io_conf};
static StGpioParams spi_io3 = {{ 0 }, GPIOA_BASE, 7, spi_io_conf};

const StGpioSettings i2c_io_conf = {ALT_FUNC, OPEN_DRAIN, 0, PULL_UP, 0x4};
static StGpioParams i2c1_io1 = {{ 0 }, GPIOB_BASE, 8, i2c_io_conf};
static StGpioParams i2c1_io2 = {{ 0 }, GPIOB_BASE, 9, i2c_io_conf};

void BSP_Init(Usart *usart, Spi *spi, I2c *temp_i2c, Gpio *led_gpio)
{

    // LED GPIO
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    StGpioInit(led_gpio, &led_stgpio);
    StGpioConfig(led_gpio);

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

    // SPI1
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIODEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    StGpioInit(&st_spi.scl, &spi_io1);
    StGpioInit(&st_spi.miso, &spi_io2);
    StGpioInit(&st_spi.mosi, &spi_io3);
    StGpioInit(&st_spi_cs.pin, &spi_cs_io);

    GpioCsInit(&spi->cs, &st_spi_cs, true);
    GpioCsConfig(&spi->cs);
    StSpiInit(spi, &st_spi, SPI1_BASE, &time);
    StSpiConfig(spi);

    // I2c1 PB8, PB9
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    StGpioInit(&st_i2c.scl, &i2c1_io1);
    StGpioInit(&st_i2c.sda, &i2c1_io2);

    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    StI2cInit(temp_i2c, &st_i2c, I2C1_BASE, &time);
    StI2cConfig(temp_i2c, 0x20B);
}

void USART3_IRQHandler(void)
{
	usart_rx_callback();
}