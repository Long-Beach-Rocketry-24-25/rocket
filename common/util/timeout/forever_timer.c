#include "retry_timer.h"

void forever_timer_start(void * data)
{
    return;
}

bool forever_timer_tick(void * data)
{
    return true;
}

void forever_timer_init(Timeout* timer)
{
    timer->start = forever_timer_start;
    timer->tick = forever_timer_tick;
}