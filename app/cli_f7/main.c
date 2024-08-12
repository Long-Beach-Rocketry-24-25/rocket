
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

RingBuffer usart3_buf;
uint8_t usart3_data[256] = {0};

Timeout time;
RetryData rd;

Usart usart3;

int main(void)
{

    BSP_Init();
    retry_timer_init(&time, &rd, 1000);
    ring_buffer_init(&usart3_buf, &usart3_data, sizeof(usart3_data));
    Usart_Init(&usart3, USART3_BASE, &time);
    Usart_Config(&usart3, SystemCoreClock, 115200);

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
        bool success = ring_buffer_pop(&usart3_buf, &data);
        if (success)
        {
            Usart_Send(&usart3, data, 1);
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
        GPIOB->ODR ^= GPIO_ODR_OD0;

        /* Delay for a period. */
        vTaskDelay(100);   
    }
}

void USART3_IRQHandler(void)
{
    // Set event flag here later.
    if (Usart_Isr_Set(&usart3, USART_ISR_RXNE))
    {
        uint8_t data = 0;
        Usart_Recv(&usart3, &data, 1);
        if (data == '\n')
        {
            
        }
        ring_buffer_insert(&usart3_buf, data);
    }
}