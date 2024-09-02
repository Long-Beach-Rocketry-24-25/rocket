
#include "bsp.h"

static StPrivUsart st_usart;
static Timeout time;
static RetryData rd;

void BSP_Init(Usart *usart)
{
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    GPIOA->MODER &= ~(GPIO_MODER_MODE5
                    | GPIO_MODER_MODE0);
    GPIOA->MODER |= (GPIO_MODER_MODE5_0
                    | GPIO_MODER_MODE0_0);
    
    GPIOA->MODER |= 1 << GPIO_MODER_MODE0_Pos;

    // Usart 1
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    GPIOB->MODER &= ~(GPIO_MODER_MODE6 | GPIO_MODER_MODE7);
    GPIOB->MODER |= (0x2 << GPIO_MODER_MODE6_Pos) | (0x2 << GPIO_MODER_MODE7_Pos);
    GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL6 | GPIO_AFRL_AFSEL7);
    GPIOB->AFR[0] |= (0x7 << GPIO_AFRL_AFSEL6_Pos) | (0x7 << GPIO_AFRL_AFSEL7_Pos);

    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority( USART1_IRQn, NVIC_EncodePriority(0, 6, 0));
    NVIC_EnableIRQ(USART1_IRQn);

    retry_timer_init(&time, &rd, 1000);
    St_Usart_Init(usart, &st_usart, USART1_BASE, &time);
    St_Usart_Config(usart, SystemCoreClock, 115200);

    // HAL_InitTick(0);
    // SystemClock_Config();

}
