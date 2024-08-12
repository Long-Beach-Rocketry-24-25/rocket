/**
 * 
 * 
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


typedef void (*start) (void * data);
typedef bool (*tick) (void * data);

typedef struct
{
    start start;
    tick tick;
    void * data;
} Timeout;
