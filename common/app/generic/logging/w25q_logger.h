
#pragma once

#include "logging.h"
#include "w25q.h"

#define W25Q_MAX_PAGE_SIZE 256

typedef struct
{
    W25q *flash;
    uint8_t end_char;
    size_t index;
    size_t max_index;
    bool allow_wrap;
} W25qLogger;

void W25qLoggerInit(LogSubscriber *sub, W25qLogger *flash_log, W25q *flash, size_t max_index);
void W25qLoggerWrapAround(LogSubscriber *sub, bool enable);
bool W25qLoggerClear(LogSubscriber *sub);
bool W25qLoggerWrite(LogSubscriber *sub, const uint8_t *data, size_t size);
bool W25qLoggerRetrieve(LogSubscriber *sub, Send *sender);
