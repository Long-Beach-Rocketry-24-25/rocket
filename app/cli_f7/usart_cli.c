
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

static TaskHandle_t cli_task;

static bool active = false;

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

bool cli_write(const char * data)
{
    size_t size = 0;
    size_t max = 60;
    while (size < max)
    {
        if (data[size] == '\0')
        {
            break;
        }
        size++;
    }
    Usart_Send(&usart, data, size);
    Usart_Send(&usart, (uint8_t*) "\n", 1);
    return true;
}

static void boot_msg()
{
    static const char* logo[] = {
        CLI_VER,
        "",
        "                       ....              ..",
        "                                           .'.",
        "        . . . .      ...;:;'.                  ..",
        "      . . .     .,coo:;,...';ldd.    ..           ..",
        "    . .     ,;;'        ,.,    ;k,      `'`        '",
        "          .o;    ':c;:;;,,,:;.   ; '     c:.         ``",
        ".       .x.    ,;'.   .:cc:ccoc    :      od;",
        ".      .l;   'c'   .;:oOKXKKOkl;   l.     ; xo         '",
        ". .   ;c    :;   .:. cKNNWNXKd; ; :    .l:.        .",
        ". .  .;.    ;,   'l. .xkOOkOxl, .    .:oc",
        "   .  ;,'     ,.  .o,  .':,..  . . : c.             THE",
        "   .  .cc'    ''   :x'     .,:;' ' .        INDOMITABLE",
        "        ;c.     .'   ;xc.                  HUMAN SPIRIT",
        "         .'...    ..  .lo:,... ....;;'.   SHALL PREVAIL",
        "            ...         ..';",
        "              .;;..",
        "                  ..",
        "",
        "",
    };
    for (int i = 0; i < 21; ++i)
    {
        cli_write(logo[i]);
    }
}

static void cli_process_task(void * params)
{
    const TickType_t max_block_time = pdMS_TO_TICKS(UINT32_MAX);
    BaseType_t rx_cplt;

    cli_write("enter pass: ");

    for ( ;; )
    {
        rx_cplt = xTaskNotifyWait( pdFALSE,
                                 UINT32_MAX,
                                 NULL,
                                 max_block_time);
        if (rx_cplt == pdPASS)
        {
            char command[MAX_CMD_LENGTH];
            size_t num_chars = get_string_from_buf(&usart_buf, command, MAX_CMD_LENGTH);
            command[num_chars] = '\0';

            if (active)
            {
                cli_process(command);
                cli_write("root:~$: ");
            }
            else
            {
                if (strcmp(command, "LBR") == 0)
                {
                    active = true;
                    cli_write("Password accepted, booting...");
                    boot_msg();
                    cli_write("root:~$: ");
                }
                else
                {
                    cli_write("Bad password, try again.");
                    cli_write("enter pass: ");
                }
            }
        }
        vTaskDelay(100);
    }
}

void usart_rx_callback()
{
    if (Usart_Isr_Set(&usart, USART_ISR_RXNE))
    {
        BaseType_t higher_prio_task_woken = pdFALSE;
        uint8_t data = 0;
        Usart_Recv(&usart, &data, 1);
        ring_buffer_insert(&usart_buf, data);
        if (data == '\n')
        {
            xTaskNotifyFromISR(cli_task, 0, eNoAction,
                               &higher_prio_task_woken);
        }
        portYIELD_FROM_ISR(higher_prio_task_woken);
    }
}

bool create_cli_task(uint32_t usart_base_addr, uint32_t sys_core_clk,
                     Command * commands, size_t num_commands)
{
    retry_timer_init(&time, &rd, 1000);
    ring_buffer_init(&usart_buf, &usart_data, sizeof(usart_data));
    Usart_Init(&usart, usart_base_addr, &time);
    Usart_Config(&usart, sys_core_clk, 115200);

    send.write_str = cli_write;
    cli_init(&send);

    for (size_t i = 0; i < num_commands; ++i)
    {
        cli_register_command(&commands[i]);
    }
    cli_task = xTaskCreateStatic( cli_process_task, "CLI", 50, NULL, 1, cli_task_stack, &cli_task_buffer);
}
