
#include "FreeRTOS.h"
#include "task.h"

#include "bsp.h"

#include "usart.h"
#include "usart_cli.h"

/* The task functions. */
void vTask1( void * pvParameters );

/*-----------------------------------------------------------*/

StackType_t task1_stack[50];
StaticTask_t task1_buffer;

/*-----------------------------------------------------------*/

void blink(int argc, char* argv[]);

Usart usart;

int main(void)
{

    BSP_Init(&usart);

    Command commands[2] = { 
        {"Blink", blink, "Blinks LED."}
    };
    create_cli_task(&usart, commands, 1);

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    for( ; ; )
    {
    }

    return 0;
}


void vTask1( void * pvParameters )
{

    for ( ; ; )
    {

        /* Delay for a period. */
        vTaskDelay(10);
    }
}

void blink(int argc, char* argv[])
{
    GPIOB->ODR ^= GPIO_ODR_OD0;
    cli_write("Blink - %d", GPIOB->ODR & GPIO_ODR_OD0);
}

void USART3_IRQHandler(void)
{
    usart_rx_callback();
}