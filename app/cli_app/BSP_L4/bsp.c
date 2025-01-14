
#include "bsp.h"

static StPrivUsart st_usart;
static StPrivI2c st_i2c1;
static StGpioParams led_stgpio = {{ 0 }, GPIOA_BASE, 5, {GPOUT, 0, 0, 0, 0}};

// Sequential use of these, so using one is fine. Not thread safe.
static Timeout time;
static FrtTimerData frt;

static StGpioParams uart_io1 = {{ 0 }, GPIOA_BASE, 2,
                                {ALT_FUNC, 0, 0, 0, 0x7}}; // USART2 AF 7
static StGpioParams uart_io2 = {{ 0 }, GPIOA_BASE, 3,
                                {ALT_FUNC, 0, 0, 0, 0x7}}; // USART2 AF 7

const StGpioSettings i2c_io_conf = {ALT_FUNC, OPEN_DRAIN, 0, PULL_UP, 0x4};

static StGpioParams i2c1_io1 = {{ 0 }, GPIOB_BASE, 8, i2c_io_conf};
static StGpioParams i2c1_io2 = {{ 0 }, GPIOB_BASE, 9, i2c_io_conf};

void BSP_Init(Usart *usart, I2c *temp_i2c, Gpio *led_gpio)
{

    HAL_InitTick(0);
    SystemClock_Config();

    // LED GPIO
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    StGpioInit(led_gpio, &led_stgpio);
    StGpioConfig(led_gpio);

    // Single FreeRTOS timer
    frt_timer_init(&time, &frt, 100);

    // UART4
    // RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    StGpioInit(&st_usart.rx, &uart_io1);
    StGpioInit(&st_usart.tx, &uart_io2);

    RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;

    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(0, 6, 0));
    NVIC_EnableIRQ(USART2_IRQn);

    StUsartInit(usart, &st_usart, USART2_BASE, &time);
    StUsartConfig(usart, SystemCoreClock, 115200);

    // I2c1 PB8, PB9
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

    StGpioInit(&st_i2c1.scl, &i2c1_io1);
    StGpioInit(&st_i2c1.sda, &i2c1_io2);

    RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;

    StI2cInit(temp_i2c, &st_i2c1, I2C1_BASE, &time);
    StI2cConfig(temp_i2c, 0x10909CEC);
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the RCC Oscillators according to the specified parameters
    in the RCC_OscInitTypeDef structure.*/
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 10;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
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
        GPIOA->ODR |= GPIO_ODR_OD0;
    }
    /* USER CODE END Error_Handler_Debug */
}