#include "stm32l4xx.h"
#include "stm32l4xx_hal.h"

#include "usart_cli.h"

extern TIM_HandleTypeDef htim1;

// void UART4_IRQHandler(void)
// {
//     usart_rx_callback();
// }

void USART1_IRQHandler(void)
{
    usart_rx_callback();
}

void TIM1_UP_TIM16_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim1);
}

/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
	while (1)
	{
	}
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
	while (1)
	{
	}
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
	while (1)
	{
	}
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
	while (1)
	{
	}
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
	while (1)
	{
	}
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
}