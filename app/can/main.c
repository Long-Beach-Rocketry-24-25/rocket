
#include "FreeRTOS.h"
#include "task.h"

#include "bsp.h"


/* The task functions. */
void vTask1( void * pvParameters );
void vTask2( void * pvParameters );

/*-----------------------------------------------------------*/

StackType_t task1_stack[50];
StackType_t task2_stack[50];
StaticTask_t task1_buffer;
StaticTask_t task2_buffer;

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
        /* Do something. */
        GPIOA->ODR ^= GPIO_ODR_OD5;

        /* Delay for a period. */
        vTaskDelay(100);
    }
}

void vTask2( void * pvParameters )
{
    for( ; ; )
    {
        /* Do something. */
        GPIOA->ODR ^= GPIO_ODR_OD0;

        /* Delay for a period. */
        vTaskDelay(100);   
    }
}