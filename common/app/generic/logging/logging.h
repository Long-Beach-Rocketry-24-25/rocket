
#pragma once

#include "send.h"

#include <stdint.h>

typedef struct LogSubscriber LogSubscriber;

struct LogSubscriber
{
    bool (*clear)(LogSubscriber* sub);
    bool (*write)(LogSubscriber* sub, const uint8_t* data, size_t size);
    bool (*retrieve_all)(LogSubscriber* sub, Send* sender);
    void* priv;
};

typedef struct LogBuilder LogBuilder;

struct LogBuilder
{
    bool (*build_new)(LogBuilder* builder);
    const uint8_t* (*get_ptr)(LogBuilder* builder);
    size_t (*get_size)(LogBuilder* builder);
    void* priv;
};

typedef struct Logger Logger;

struct Logger
{
    LogBuilder* builder;
    LogSubscriber* subs;
    size_t num_subs;
    Send* comm;
    bool cmd_enable;
    bool enabled;
};

void logger_init(Logger* log, LogBuilder* builder, LogSubscriber* subs,
                 size_t num_subs, Send* comm);
bool logger_update(Logger* log);
void logger_enable(Logger* log, bool enable);
bool logger_retrieve(Logger* log);
