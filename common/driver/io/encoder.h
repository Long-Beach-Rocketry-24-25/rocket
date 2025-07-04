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

    /**
     * Gets the number of expected counts per full encoder
     * A/B cycle (for example, rising and falling for both
     * A and B signal edges would equate to 4 counts.)
     * 
     * @param encoder the encoder instance.
     * @return The number of counts per cycle.
     */
    size_t (*counts_per_cycle)(Encoder* encoder);

    /**
     * Private user context for concrete implementations to use.
     */
    void* priv;
};