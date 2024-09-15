
#include "FreeRTOS.h"
#include "task.h"

#include "bsp.h"

#include "cli.h"
#include "st_usart.h"
#include "forever_timer.h"
#include "ring_buffer.h"

/* The task functions. */
void vTask1( void * pvParameters );
void vTask2( void * pvParameters );

/*-----------------------------------------------------------*/

StackType_t task1_stack[50];
StackType_t task2_stack[50];
StaticTask_t task1_buffer;
StaticTask_t task2_buffer;

/*-----------------------------------------------------------*/

RingBuffer usart1_buf;
uint8_t usart1_data[256] = {0};

Timeout time;
StPrivUsart st_usart;

Usart usart1;

int main(void)
{

    BSP_Init();
    forever_timer_init(&time);
    ring_buffer_init(&usart1_buf, &usart1_data, sizeof(usart1_data));
    St_Usart_Init(&usart1, &st_usart, USART1_BASE, &time);
    St_Usart_Config(&usart1, SystemCoreClock, 115200);

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

    uint8_t data[6] = {"hello\n"};

    for ( ; ; )
    {
        // Do event flag here later
        uint8_t data[2] = {0};
        bool success = ring_buffer_pop(&usart1_buf, &data);
        if (success)
        {
            usart1.send(&usart1, data, 1);
        }

        /* Delay for a period. */
        vTaskDelay(10);
    }
}

void vTask2( void * pvParameters )
{
    for( ; ; )
    {
        /* Do something. */
        // GPIOA->ODR ^= GPIO_ODR_OD0;

        /* Delay for a period. */
        vTaskDelay(100);   
    }
}

void USART1_IRQHandler(void)
{
    if (usart1.rx_ready(&usart1))
    {
        uint8_t data = 0;
        usart1.recv(&usart1, &data, 1);
        ring_buffer_insert(&usart1_buf, data);
    }
}