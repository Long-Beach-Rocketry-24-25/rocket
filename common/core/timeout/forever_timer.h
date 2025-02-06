/**
 * Basic implementation of timer interface.
 * Never times out.
 * 
 * Author: Nick Fan
 */

#pragma once

#include "timeout.h"

/**
 * Start function for Timeout interface.
 */
void forever_timer_start(void* data);

/**
 * Tick function for Timeout interface.
 */
bool forever_timer_tick(void* data);

/**
 * Initializes a Timeout interface as a Forever Timer, that
 * never expires.
 * 
 * @param timer the timeout interface to initialize.
 */
void forever_timer_init(Timeout* timer);