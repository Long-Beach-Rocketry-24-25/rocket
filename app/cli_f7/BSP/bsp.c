
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

const StGpioConfig i2c_io_conf = {ALT_FUNC, OPEN_DRAIN, 0, PULL_UP, 0x4};

static StGpioParams i2c1_io1 = {{ 0 }, GPIOB_BASE, 8, i2c_io_conf};
static StGpioParams i2c1_io2 = {{ 0 }, GPIOB_BASE, 9, i2c_io_conf};

void BSP_Init(Usart *usart, I2c *temp_i2c, Gpio *led_gpio)
{

    // LED GPIO
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    St_Gpio_Init(led_gpio, &led_stgpio);
    St_Gpio_Config(led_gpio);

    // Single FreeRTOS timer
    frt_timer_init(&time, &frt, 100);

    // USART3
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

    St_Gpio_Init(&st_usart.rx, &uart_io1);
    St_Gpio_Init(&st_usart.tx, &uart_io2);

    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(0, 6, 0));
    NVIC_EnableIRQ(USART3_IRQn);

    St_Usart_Init(usart, &st_usart, USART3_BASE, &time);
    St_Usart_Config(usart, SystemCoreClock, 115200);

    // I2c1 PC8, PC9
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    St_Gpio_Init(&st_i2c.scl, &i2c1_io1);
    St_Gpio_Init(&st_i2c.sda, &i2c1_io2);

    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    St_I2c_Init(temp_i2c, &st_i2c, I2C1_BASE, &time);
    St_I2c_Config(temp_i2c, 0x20B);
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 72;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 3;
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
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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