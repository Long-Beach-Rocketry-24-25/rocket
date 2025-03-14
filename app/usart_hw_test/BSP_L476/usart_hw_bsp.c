
#include "usart_hw_bsp.h"
#include "stm32l476xx.h"

static StPrivUsart st_cli_usart;
static StPrivUsart st_comm_usart;
static StGpioParams led_stgpio = {{0}, GPIOA_BASE, 5, {GPOUT, 0, 0, 0, 0}};
static StGpioParams txe_gpio = {{0}, GPIOC_BASE, 12, {1, 0, 0, 0, 0}};
static StGpioParams rxe_gpio = {{0}, GPIOD_BASE, 2, {1, 0, 0, 0, 0}};

// Sequential use of these, so using one is fine. Not thread safe.
static Timeout time;
static FrtTimerData frt;

static StGpioParams cli_uart_io1 = {{0},
                                    GPIOA_BASE,
                                    2,
                                    {ALT_FUNC, 0, 0, 0, 0x7}};  // USART2 AF 7
static StGpioParams cli_uart_io2 = {{0},
                                    GPIOA_BASE,
                                    3,
                                    {ALT_FUNC, 0, 0, 0, 0x7}};  // USART2 AF 7

static StGpioParams comm_uart_io1 = {{0},
                                     GPIOC_BASE,
                                     10,
                                     {ALT_FUNC, 0, 0, 0, 0x7}};  // USART3 AF 7
static StGpioParams comm_uart_io2 = {{0},
                                     GPIOC_BASE,
                                     11,
                                     {ALT_FUNC, 0, 0, 0, 0x7}};  // USART3 AF 7

static RingBuffer rb1;
static RingBuffer rb2;
static uint8_t arr1[UART_PIPE_BUF_SIZE] = {0};
static uint8_t arr2[UART_PIPE_BUF_SIZE] = {0};

Snx5176b rs485;

void BSP_Init(Usart* cli_usart, Usart* comm_usart, Gpio* led_gpio)
{
    HAL_InitTick(0);
    SystemClock_Config();

    // LED GPIO
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    StGpioInit(led_gpio, &led_stgpio);
    StGpioConfig(led_gpio);

    // Single FreeRTOS timer
    frt_timer_init(&time, &frt, 100);

    // Cli USART2
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    StGpioInit(&st_cli_usart.rx, &cli_uart_io1);
    StGpioInit(&st_cli_usart.tx, &cli_uart_io2);

    RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;

    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(0, 6, 0));
    NVIC_EnableIRQ(USART2_IRQn);

    StUsartInit(cli_usart, &st_cli_usart, USART2_BASE, &time);
    StUsartConfig(cli_usart, SystemCoreClock, 115200);

    // Comm USART3
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;

    StGpioInit(&st_comm_usart.rx, &comm_uart_io1);
    StGpioInit(&st_comm_usart.tx, &comm_uart_io2);

    RCC->APB1ENR1 |= RCC_APB1ENR1_USART3EN;

    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(0, 6, 0));
    NVIC_EnableIRQ(USART3_IRQn);

    StUsartInit(comm_usart, &st_comm_usart, USART3_BASE, &time);
    StUsartConfig(comm_usart, SystemCoreClock, 115200);

    ring_buffer_init(&rb1, arr1, UART_PIPE_BUF_SIZE);
    ring_buffer_init(&rb2, arr2, UART_PIPE_BUF_SIZE);

    StGpioInit(&rs485.txe, &txe_gpio);
    StGpioConfig(&rs485.txe);

    StGpioInit(&rs485.rxe, &rxe_gpio);
    StGpioConfig(&rs485.rxe);

    Snx5176bInit(comm_usart, &rs485);
    Snx5176bConfig(&rs485);

    UartPipeInit(cli_usart, comm_usart, &rb1, &rb2, '\n');
}

void USART2_IRQHandler(void)
{
    UartPipeCallback1();
}

void USART3_IRQHandler(void)
{
    UartPipeCallback2();
}

void TIM1_UP_TIM16_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim1);
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
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
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