#include "retry_timer.h"

void retry_timer_start(void * data)
{
    RetryData* retry_data = (RetryData *) data;
    retry_data->counter = retry_data->reset;
}

bool retry_timer_tick(void * data)
{
    RetryData* retry_data = (RetryData *) data;
    if (retry_data->counter - 1 <= 0)
    {
        retry_data->counter--;
        return false;
    }
    return true;

}

void retry_timer_init(Timeout* timer, RetryData* retry, size_t count)
{
    retry->reset = count;
    retry->counter = 0;

    timer->data = (void *) retry;
    timer->start = retry_timer_start;
    timer->tick = retry_timer_tick;
}