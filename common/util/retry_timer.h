/**
 * Basic implementation of timer interface.
 * Times out after counting x iterations.
 * 
 * Author: Nick Fan
 */

#pragma once

#include "timeout.h"

/**
 * Data necessary for each timer instance.
 */
typedef struct
{
    size_t reset;
	volatile size_t counter;
} RetryData;

/**
 * Start function for Timeout interface.
 */
void retry_timer_start(void * data)
{
    RetryData* retry_data = (RetryData *) data;
    retry_data->counter = retry_data->reset;
}

/**
 * Tick function for Timeout interface.
 */
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

/**
 * Initializes a Timeout interface as a Retry Timer, that expires
 * after checking the condition for 'count' iterations.
 * 
 * @param timer the timeout interface to initialize.
 * @param retry the data for a retry timer to use for the instance.
 * @param count the number of iterations before the timer expires each time.
 */
void retry_timer_init(Timeout* timer, RetryData* retry, size_t count)
{
    retry->reset = count;
    retry->counter = 0;

    timer->data = (void *) retry;
    timer->start = retry_timer_start;
    timer->tick = retry_timer_tick;
}