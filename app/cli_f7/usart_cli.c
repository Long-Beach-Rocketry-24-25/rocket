
#include "usart_cli.h"

#define MSG_READY 0x1

static StackType_t cli_task_stack[50];
static StaticTask_t cli_task_buffer;

static RingBuffer usart_buf;
static uint8_t usart_data[256] = {0};

static Timeout time;
static RetryData rd;
static Usart usart;
static Send send;

static bool msg_ready;

static size_t get_string_from_buf(RingBuffer *buf, char *string, size_t max)
{
    for (size_t i = 0; i < max; ++i)
    {
        /*
         * Evaluates left to right.
         */
        uint8_t data = 0;
        bool success = ring_buffer_pop(buf, &data);
        string[i] = (char) data;
        if (!success || (data == '\n'))
        {
            return i;
        }
    }
}

static bool write_str(const char * data, size_t size)
{
    Usart_Send(&usart, data, size);
    Usart_Send(&usart, (uint8_t *)"\n", 1);
}

static void cli_process_task(void * params)
{
    
    for ( ;; )
    {
        if (msg_ready)
        {
            msg_ready = false;
            char command[MAX_CMD_LENGTH];

            size_t num_chars = get_string_from_buf(&usart_buf, command, MAX_CMD_LENGTH);
            command[num_chars] = '\0';
            cli_process(command);
        }
        vTaskDelay(100);
    }
}

void usart_rx_callback()
{
    if (Usart_Isr_Set(&usart, USART_ISR_RXNE))
    {
        uint8_t data = 0;
        Usart_Recv(&usart, &data, 1);
        ring_buffer_insert(&usart_buf, data);
        if (data == '\n')
        {
            msg_ready = true;
        }
    }
}

bool create_cli_task(uint32_t usart_base_addr, uint32_t sys_core_clk,
                     Command * commands, size_t num_commands)
{
    retry_timer_init(&time, &rd, 1000);
    ring_buffer_init(&usart_buf, &usart_data, sizeof(usart_data));
    Usart_Init(&usart, usart_base_addr, &time);
    Usart_Config(&usart, sys_core_clk, 115200);

    send.write_str = write_str;
    cli_init(&send);

    for (size_t i = 0; i < num_commands; ++i)
    {
        cli_register_command(&commands[i]);
    }
    xTaskCreateStatic( cli_process_task, "CLI", 50, NULL, 1, cli_task_stack, &cli_task_buffer);
}
