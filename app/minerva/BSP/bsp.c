
#include "bsp.h"

static StPrivUsart st_usart;
static StPrivI2c st_i2c1;
static StPrivI2c st_i2c2;
static StPrivI2c st_i2c3;
static StBxCanParams st_bxcan = {{ 0 }, CAN1_BASE, 0, 0, { 1, 15, 2, 4 }, { CAN_STANDARD_ID, 0x1 }};
static StGpioParams led_stgpio = {{ 0 }, GPIOH_BASE, 3, {1, 0, 0, 0, 0}};

// Sequential use of these, so using one is fine. Not thread safe.
static Timeout time;
static FrtTimerData frt;

static StGpioParams uart_io1 = {{ 0 }, GPIOA_BASE, 0, 
                                {ALT_FUNC, 0, 0, 0, 0x8}}; // UART4 AF 8
static StGpioParams uart_io2 = {{ 0 }, GPIOA_BASE, 1,
                                {ALT_FUNC, 0, 0, 0, 0x8}}; // UART4 AF 8

static StGpioParams can_io1 = {{ 0 }, GPIOB_BASE, 12, 
                              {ALT_FUNC, 0, VERY_HIGH, PULL_UP, 0xA}}; // CAN1 AF 10
static StGpioParams can_io2 = {{ 0 }, GPIOB_BASE, 13,
                              {ALT_FUNC, 0, VERY_HIGH, PULL_UP, 0xA}}; // CAN1 AF 10

const StGpioConfig i2c_io_conf = {ALT_FUNC, OPEN_DRAIN, 0, PULL_UP, 0x4};

static StGpioParams i2c1_io1 = {{ 0 }, GPIOB_BASE, 8, i2c_io_conf};
static StGpioParams i2c1_io2 = {{ 0 }, GPIOB_BASE, 9, i2c_io_conf};

static StGpioParams i2c2_io1 = {{ 0 }, GPIOB_BASE, 10, i2c_io_conf};
static StGpioParams i2c2_io2 = {{ 0 }, GPIOB_BASE, 11, i2c_io_conf};

static StGpioParams i2c3_io1 = {{ 0 }, GPIOC_BASE, 0, i2c_io_conf};
static StGpioParams i2c3_io2 = {{ 0 }, GPIOC_BASE, 1, i2c_io_conf};

void BSP_Init(Usart *usart, I2c *temp_i2c, I2c *an1_i2c, I2c *an2_i2c, CanBus *can, Gpio *led_gpio)
{

    HAL_InitTick(0);
    SystemClock_Config();

    // LED GPIO
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOHEN;

    St_Gpio_Init(led_gpio, &led_stgpio);
    St_Gpio_Config(led_gpio);

    // Single FreeRTOS timer
    frt_timer_init(&time, &frt, 100);

    // UART4
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    St_Gpio_Init(&st_usart.rx, &uart_io1);
    St_Gpio_Init(&st_usart.tx, &uart_io2);

    RCC->APB1ENR1 |= RCC_APB1ENR1_UART4EN;

    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority(UART4_IRQn, NVIC_EncodePriority(0, 6, 0));
    NVIC_EnableIRQ(UART4_IRQn);

    St_Usart_Init(usart, &st_usart, UART4_BASE, &time);
    St_Usart_Config(usart, SystemCoreClock, 115200);

    // CAN1
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

    St_Gpio_Init(&st_bxcan.priv.rx, &can_io1);
    St_Gpio_Init(&st_bxcan.priv.tx, &can_io2);

    RCC->APB1ENR1 |= RCC_APB1ENR1_CAN1EN;

    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority(CAN1_RX0_IRQn, NVIC_EncodePriority(0, 6, 0));
    NVIC_EnableIRQ(CAN1_RX0_IRQn);

    St_BxCan_Init(can, &st_bxcan, &time);
    St_BxCan_Config(can);

    // I2c1 PB8, PB9
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

    St_Gpio_Init(&st_i2c1.scl, &i2c1_io1);
    St_Gpio_Init(&st_i2c1.sda, &i2c1_io2);

    RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;

    St_I2c_Init(an1_i2c, &st_i2c1, I2C1_BASE, &time);
    St_I2c_Config(an1_i2c, 0x20B);

    // I2c2 PB10, PB11
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

    St_Gpio_Init(&st_i2c2.scl, &i2c2_io1);
    St_Gpio_Init(&st_i2c2.sda, &i2c2_io2);

    RCC->APB1ENR1 |= RCC_APB1ENR1_I2C2EN;

    St_I2c_Init(temp_i2c, &st_i2c2, I2C2_BASE, &time);
    St_I2c_Config(temp_i2c, 0x20B);

    // I2c3 PC0 PC1
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

    St_Gpio_Init(&st_i2c3.scl, &i2c3_io1);
    St_Gpio_Init(&st_i2c3.sda, &i2c3_io2);

    RCC->APB1ENR1 |= RCC_APB1ENR1_I2C3EN;

    St_I2c_Init(an2_i2c, &st_i2c3, I2C3_BASE, &time);
    St_I2c_Config(an2_i2c, 0x20B);
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
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 8;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV8;
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

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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