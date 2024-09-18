
#include "bsp.h"

static StPrivUsart st_usart;
static StPrivI2c st_i2c;
static Timeout usart_time;
static FrtTimerData usart_frt;
static Timeout i2c_time;
static FrtTimerData i2c_frt;
static StPrivGpio led_stgpio;

static Gpio uart_1_gpio;
static Gpio uart_2_gpio;
static StPrivGpio uart_1_stgpio;
static StPrivGpio uart_2_stgpio;

static Gpio i2c_1_gpio;
static Gpio i2c_2_gpio;
static StPrivGpio i2c_1_stgpio;
static StPrivGpio i2c_2_stgpio;

void BSP_Init(Usart *usart, I2c *i2c, Gpio *led_gpio)
{
    HAL_InitTick(0);
    SystemClock_Config();

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    St_Gpio_Init(led_gpio, &led_stgpio, GPIOA_BASE, 5);

    StGpioConfig led_conf = {
        1,
        0,
        0,
        0,
        0
    };
    St_Gpio_Config(led_gpio, &led_conf);

    // Usart 1
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

    St_Gpio_Init(&uart_1_gpio, &uart_1_stgpio, GPIOB_BASE, 6);
    St_Gpio_Init(&uart_2_gpio, &uart_2_stgpio, GPIOB_BASE, 7);

    StGpioConfig uart_io_conf = {
        ALT_FUNC,
        0,
        0,
        0,
        0x7
    };
    St_Gpio_Config(&uart_1_gpio, &uart_io_conf);
    St_Gpio_Config(&uart_2_gpio, &uart_io_conf);

    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority( USART1_IRQn, NVIC_EncodePriority(0, 6, 0));
    NVIC_EnableIRQ(USART1_IRQn);

    // I2c 1 PC8 PC9

    RCC->APB1LENR |= RCC_APB1LENR_I2C1EN;
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

    St_Gpio_Init(&i2c_1_gpio, &i2c_1_stgpio, GPIOC_BASE, 8);
    St_Gpio_Init(&i2c_2_gpio, &i2c_2_stgpio, GPIOC_BASE, 9);

    StGpioConfig i2c_io_conf = {
        ALT_FUNC,
        OPEN_DRAIN,
        0,
        PULL_UP,
        0x4
    };
    St_Gpio_Config(&i2c_1_gpio, &i2c_io_conf);
    St_Gpio_Config(&i2c_2_gpio, &i2c_io_conf);

    frt_timer_init(&usart_time, &usart_frt, 500);
    St_Usart_Init(usart, &st_usart, USART1_BASE, &usart_time);
    St_Usart_Config(usart, SystemCoreClock, 115200);

    frt_timer_init(&i2c_time, &i2c_frt, 100);
    St_I2c_Init(i2c, &st_i2c, I2C1_BASE, &i2c_time);
    St_I2c_Config(i2c, 0x60808CD3);

}


void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

    while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLL1_SOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 12;
    RCC_OscInitStruct.PLL.PLLN = 250;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLQ = 10;
    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1_VCIRANGE_1;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1_VCORANGE_WIDE;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                                |RCC_CLOCKTYPE_PCLK3;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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