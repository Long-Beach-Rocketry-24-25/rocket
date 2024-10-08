/**
 * 
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    CAN_STANDARD_ID,
    CAN_EXTENDED_ID
} CanIdType;

typedef struct
{
    CanIdType type;
    uint32_t id;
} CanId;

typedef struct CanBus CanBus;

struct CanBus
{
    void (*set_id) (CanBus *can, CanId id);
    // void (*add_filter) (CanBus *can, CanId id);
    bool (*send) (CanBus *can, uint8_t *data, size_t size);
    bool (*recv) (CanBus *can, uint8_t *data, size_t size);

    void * priv;
};