/**
 * Basic implementation of timer interface.
 * Times out after counting x iterations.
 * 
 * Author: Nick Fan
 */

#pragma once

#include "timeout.h"

#include <stddef.h>

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
void retry_timer_start(void* data);

/**
 * Tick function for Timeout interface.
 */
bool retry_timer_tick(void* data);

/**
 * Initializes a Timeout interface as a Retry Timer, that expires
 * after checking the condition for 'count' iterations.
 * 
 * @param timer the timeout interface to initialize.
 * @param retry the data for a retry timer to use for the instance.
 * @param count the number of iterations before the timer expires each time.
 */
void retry_timer_init(Timeout* timer, RetryData* retry, size_t count);