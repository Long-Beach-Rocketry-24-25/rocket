
#include "FreeRTOS.h"
#include "task.h"

#include "bsp.h"

#include "usart.h"
#include "usart_cli.h"

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

void blink(int argc, char* argv[])
{
    GPIOA->ODR ^= GPIO_ODR_OD5;
    cli_write("Blink - %d", !!(GPIOA->ODR & GPIO_ODR_OD5));
}