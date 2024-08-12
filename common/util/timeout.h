/**
 * Timing utility interface. Utilizes a macro function
 * to allow returning a desired abort value on timeout.
 * If the desired condition is met before the timer expires,
 * the program will continue without aborting.
 * 
 * Author: Nick Fan
 */


#pragma once

#include <stdbool.h>

/**
 * Starts timer and waits for cond to become true,
 * otherwise returns ret if the timer expires.
 * Equivalent to while(!(cond)) with a timeout.
 *
 * @param timer The timeout to use.
 * @param cond The condition to wait for to become true.
 * @param ret The value to return if time runs out.
 *
 */
#define WAIT(timer, cond, ret)          \
do							            \
{							            \
	timer->start(timer->data);          \
	while(!(cond))			            \
	{						            \
		if (!timer->tick(timer->data))  \
		{                               \
			return ret;                 \
		}                               \
	}                                   \
} while (0)

/**
 * Resets and "starts" the timer.
 * 
 * @param data the timer data used to determine expiration,
 *             passed as the only argument of the function.
 */
typedef void (*start) (void * data);

/**
 * To be called every iteration of the waiting loop. This
 * can be implemented as a real time-based check or primitive
 * countdown timer.
 * 
 * @param data the timer data used to determine expiration,
 *             passed as the only argument of the function.
 * 
 * @returns True if still ticking, or false if expired.
 */
typedef bool (*tick) (void * data);

/**
 * Timeout interface.
 */
typedef struct
{
    /**
     * Reset and "start" the timer.
     */
    start start;

    /**
     * Checks if timer has expired, returns true
     * if alive or false if expired.
     */
    tick tick;

    /**
     * Implementation specific information for
     * the timer instance.
     */
    void * data;

} Timeout;
