
#include "FreeRTOS.h"
#include "task.h"

#include "bsp.h"

#include "usart_cli.h"

/* The task functions. */
void vTask1( void * pvParameters );
void vTask2( void * pvParameters );

/*-----------------------------------------------------------*/

StackType_t task1_stack[50];
StackType_t task2_stack[50];
StaticTask_t task1_buffer;
StaticTask_t task2_buffer;

/*-----------------------------------------------------------*/

void blink(int argc, char* argv[]);

int main(void)
{

    BSP_Init();

    xTaskCreateStatic( vTask1,   /* Pointer to the function that implements the task. */
                       "Task 1", /* Text name for the task. */
                       50,     /* Stack depth */
                       NULL,     /* We are not using the task parameter. */
                       1,        /* This task will run at priority 1. */
                       task1_stack,
                       &task1_buffer);


    /* Create the other task in exactly the same way. */
    xTaskCreateStatic( vTask2, "Task 2", 50, NULL, 1, task2_stack, &task2_buffer);


    Command commands[2] = { {"Blink", blink, "Blinks LED."} };
    create_cli_task(USART3_BASE, SystemCoreClock, commands, 1);

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    for( ; ; )
    {
    }

    return 0;
}


void vTask1( void * pvParameters )
{

    uint8_t data[6] = {"hello\n"};

    for ( ; ; )
    {

        /* Delay for a period. */
        vTaskDelay(10);
    }
}

void vTask2( void * pvParameters )
{
    for( ; ; )
    {
        /* Do something. */

        /* Delay for a period. */
        vTaskDelay(100);   
    }
}

void blink(int argc, char* argv[])
{
    GPIOB->ODR ^= GPIO_ODR_OD0;
}

void USART3_IRQHandler(void)
{
    usart_rx_callback();
}