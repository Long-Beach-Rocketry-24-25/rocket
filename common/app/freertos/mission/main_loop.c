
#include "main_loop.h"

static StackType_t main_loop_stack[MAIN_LOOP_STACK_SIZE_BYTES];
static StaticTask_t main_loop_buffer;

static TaskHandle_t main_loop;

void (*callback) (void);
TickType_t target_loop_ms;

static void loop_func(void * params)
{
    while (1)
    {
        TickType_t start = xTaskGetTickCount();

        callback();

        TickType_t time = xTaskGetTickCount() - start;
        if (time < target_loop_ms)
        {
            vTaskDelay(target_loop_ms - time);
        }
    }
}

void create_main_loop(void (*func) (void), size_t frequency_hz)
{
    callback = func;
    target_loop_ms = 1000 / frequency_hz;
    main_loop = xTaskCreateStatic(loop_func, "Main Loop", MAIN_LOOP_STACK_SIZE_BYTES, NULL, 1,
                                     main_loop_stack, &main_loop_buffer);
}
