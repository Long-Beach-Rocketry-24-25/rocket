
#include "bsp.h"

static StPrivUsart st_usart;
static StPrivI2c st_i2c;
static Timeout usart_time;
static FrtTimerData usart_frt;
static Timeout i2c_time;
static FrtTimerData i2c_frt;

void BSP_Init(Usart *usart, I2c *i2c)
{
    // HAL_InitTick(0);
    // SystemClock_Config();

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOBEN;

    // Usart 3
    GPIOD->MODER &= ~(GPIO_MODER_MODER8 | GPIO_MODER_MODER9);
    GPIOD->MODER |= (0x2 << GPIO_MODER_MODER8_Pos) | (0x2 << GPIO_MODER_MODER9_Pos);
    GPIOD->AFR[1] &= ~(GPIO_AFRH_AFRH0 | GPIO_AFRH_AFRH1);
    GPIOD->AFR[1] |= (0x7 << GPIO_AFRH_AFRH0_Pos) | (0x7 << GPIO_AFRH_AFRH1_Pos);

    GPIOB->MODER |= 1 << GPIO_MODER_MODER0_Pos;

    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

    // I2c1 PB8 PB9
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    GPIOB->MODER &= ~(GPIO_MODER_MODER8 | GPIO_MODER_MODER9);
    GPIOB->MODER |= (0x2 << GPIO_MODER_MODER8_Pos) | (0x2 << GPIO_MODER_MODER9_Pos);

    GPIOB->OTYPER |= (GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9);

    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);
    GPIOB->PUPDR |= (0x1 << GPIO_PUPDR_PUPDR8_Pos) | (0x1 << GPIO_PUPDR_PUPDR9_Pos);

    GPIOB->AFR[1] &= ~(GPIO_AFRH_AFRH0 | GPIO_AFRH_AFRH1);
    GPIOB->AFR[1] |= (0x4 << GPIO_AFRH_AFRH0_Pos) | (0x4 << GPIO_AFRH_AFRH1_Pos);

    // Usart interrupt
    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority( USART3_IRQn, NVIC_EncodePriority(0, 6, 0));
    NVIC_EnableIRQ(USART3_IRQn);

    frt_timer_init(&usart_time, &usart_frt, 500);
    St_Usart_Init(usart, &st_usart, USART3_BASE, &usart_time);
    St_Usart_Config(usart, SystemCoreClock, 115200);

    frt_timer_init(&i2c_time, &i2c_frt, 100);
    St_I2c_Init(i2c, &st_i2c, I2C1_BASE, &i2c_time);
    St_I2c_Config(i2c, 0x20B);

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