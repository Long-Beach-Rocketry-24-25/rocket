
#include "flight_computer_bsp.h"

#define EXIT_IF_FAIL(cond) EXIT_IF(!(cond), false)

static Mem memory;
static uint8_t driver_mem[DRIVER_MEM_SIZE] = {0};

bool BSP_Init(Usart* debug_usart, I2c* i2c, Spi* spi, Gpio* red_led,
              Gpio* green_led, Gpio* blue_led)
{
    HAL_InitTick(0);
    SystemClock_Config();

    EXIT_IF_FAIL(InitPrealloc(&memory, driver_mem, DRIVER_MEM_SIZE));

    // Single FRT timer.
    Timeout* time = make_frt_timer(&memory, 100);

    // LED GPIO
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN;

    EXIT_IF_FAIL(
        GiveStGpio(red_led, &memory,
                   (StGpioParams){{0}, GPIOC_BASE, 6, {GPOUT, 0, 0, 0, 0}}));
    EXIT_IF_FAIL(
        GiveStGpio(green_led, &memory,
                   (StGpioParams){{0}, GPIOC_BASE, 7, {GPOUT, 0, 0, 0, 0}}));
    EXIT_IF_FAIL(
        GiveStGpio(blue_led, &memory,
                   (StGpioParams){{0}, GPIOB_BASE, 0, {GPOUT, 0, 0, 0, 0}}));

    // I2c1
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;

    // PB6/7
    const StGpioSettings i2c_conf = {ALT_FUNC, OPEN_DRAIN, 0, PULL_UP, 0x4};
    EXIT_IF_FAIL(GiveStI2c(i2c, &memory, time, I2C1_BASE, 0x00C12166,
                           (StGpioParams){{0}, GPIOB_BASE, 6, i2c_conf},
                           (StGpioParams){{0}, GPIOB_BASE, 7, i2c_conf}));

    // SPI1
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    // PA4 gpio cs
    Gpio* cs_pin = MakeStGpio(
        &memory, (StGpioParams){{0}, GPIOA_BASE, 4, {1, 0, 0, 0, 0}});

    // PA 5, 6, 7 AF 5
    const StGpioSettings spi_io_conf = {ALT_FUNC, 0, 0, 0, 0x5};
    EXIT_IF_FAIL(GiveStSpi(spi, &memory, time,
                           MakeGpioCs(&memory, cs_pin, true), SPI1_BASE,
                           (StGpioParams){{0}, GPIOA_BASE, 5, spi_io_conf},
                           (StGpioParams){{0}, GPIOA_BASE, 6, spi_io_conf},
                           (StGpioParams){{0}, GPIOA_BASE, 7, spi_io_conf}));

    // USART1
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // PA9/10
    EXIT_IF_FAIL(GiveStUsart(
        debug_usart, &memory, time, USART1_BASE, SystemCoreClock, 115200,
        (StGpioParams){{0}, GPIOA_BASE, 9, {ALT_FUNC, 0, 0, 0, 0x7}},
        (StGpioParams){{0}, GPIOA_BASE, 10, {ALT_FUNC, 0, 0, 0, 0x7}}));

    return true;
}

void ConfigIrq(void)
{
    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(0, 6, 0));
    NVIC_EnableIRQ(USART1_IRQn);
}

void SystemReset(void)
{
    __NVIC_SystemReset();
}

void USART1_IRQHandler(void)
{
    cli_usart_rx_callback();
}

void TIM1_UP_TIM16_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim1);
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** 
     * Configure the main internal regulator output voltage
     */
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
    {
        Error_Handler();
    }

    /** 
     * Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 16;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** 
     * Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
        Error_Handler();
    }
}

void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}