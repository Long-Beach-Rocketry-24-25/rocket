
#include "logging_app.h"

static StackType_t log_task_stack[1024];
static StaticTask_t log_task_buffer;

static TaskHandle_t log_task;


static void log_process_task(void * params)
{

}

bool create_log_app(LogBuilder *builder, LogSubscriber *subscribers, size_t num_subs, Send *sender)
{

}