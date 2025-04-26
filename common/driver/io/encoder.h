/**
 * An interface for quadrature encoder peripherals.
 * This interface makes no promises about overflow or value range,
 * all such logic needs to be handled by the consumer of this API.
 */

#pragma once

#include <stddef.h>

/**
 * An encoder instance.
 */
typedef struct Encoder Encoder;
struct Encoder
{
    /**
     * Gets the current encoder counter value.
     * 
     * @param encoder the encoder instance.
     * @return The current counter value.
     */
    size_t (*get_counter)(Encoder* encoder);

    /**
     * Gets the maximum value of the encoder counter.
     * 
     * @param encoder the encoder instance.
     * @return The maximum encoder counter.
     */
    size_t (*get_max_value)(Encoder* encoder);
};