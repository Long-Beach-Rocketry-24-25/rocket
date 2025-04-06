
#pragma once

#include "logging.h"

#include <string.h>

typedef struct
{
    size_t divisor;
    uint8_t* buf;
    size_t buf_size;
    size_t curr_iters;
    size_t buf_idx;
    uint8_t seperator;
    LogSubscriber* internal;
} BulkLogger;

void BulkLoggerInit(LogSubscriber* sub, BulkLogger* bulk_log,
                    LogSubscriber* internal, size_t divisor, uint8_t separator,
                    uint8_t* backing_buf, size_t buf_size);
bool BulkLoggerClear(LogSubscriber* sub);
bool BulkLoggerWrite(LogSubscriber* sub, const uint8_t* data, size_t size);
bool BulkLoggerRetrieve(LogSubscriber* sub, Send* sender);