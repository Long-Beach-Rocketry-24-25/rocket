
#include "FreeRTOS.h"
#include "task.h"

#include "bsp.h"

#include "cli.h"
#include "st_usart.h"
#include "retry_timer.h"
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

RingBuffer usart2_buf;
uint8_t usart2_data[256] = {0};

Timeout time;
RetryData rd;

Usart usart2;

int main(void)
{

    BSP_Init();
    retry_timer_init(&time, &rd, 1000);
    ring_buffer_init(&usart2_buf, &usart2_data, sizeof(usart2_data));
    Usart_Init(&usart2, USART2_BASE, &time);
    Usart_Config(&usart2, SystemCoreClock, 115200);
    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority( USART2_IRQn, NVIC_EncodePriority(0, 1, 0));
    NVIC_EnableIRQ(USART2_IRQn);

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
        // Usart_Send(&usart2, data, sizeof(data));
        uint8_t data[2] = {0};
        data[1] = ring_buffer_pop(&usart2_buf, &data);
        if (data[1])
        {
            Usart_Send(&usart2, data, 1);
        }
        Usart_Send(&usart2, usart2_buf.data, 1);

        /* Delay for a period. */
        vTaskDelay(500);
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

void USART2_IRQHandler(void)
{
    GPIOA->ODR ^= GPIO_ODR_OD0;
    if (USART2->ISR & USART_ISR_RXNE)
    {
        uint8_t data = 0;
        Usart_Recv(&usart2, &data, 1);
        ring_buffer_insert(&usart2_buf, data);
        uint8_t msg[5] = {"hi\n"};
        Usart_Send(&usart2, &msg, 3);
    }
}