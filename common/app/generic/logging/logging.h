
#pragma once

#include "send.h"

#include <stdint.h>

/**
 * Loop that checks configs and acts accordingly.
 * Loop time should be statically configured.
 * 
 * Configs: Allow wrap around X see below
 * Set wrap around enable cmd X -> make this static config on LoggerSubscriber instance.
 * Enable/disable logging cmd
 * Retrieve data cmd
 * 
 * Idle: wait for enable. if enable, chip erase. set index to 0. move to active mode.
 * Active: get data iface returns serialized data, and output to subscriber ifaces (ie flash, uart).
 *         increment index by 1. if stop command, move to idle.
 * 
 * Retrieve: for each subscriber iface, call retrieve data and print on send iface
 * 
 * logging [ enable | disable | wrapEn [0 | 1] | retrieve]
 * 
 * - logging = call loggingcli.handle (argv)
 * - rest = functions registered in loggingcli
 */

typedef struct LogSubscriber LogSubscriber;

struct LogSubscriber
{
    bool (*clear) (LogSubscriber *sub);
    bool (*write) (LogSubscriber *sub, const uint8_t *data, size_t size);
    bool (*retrieve_all) (LogSubscriber *sub, Send *sender);
};

typedef struct LogBuilder LogBuilder;

struct LogBuilder
{
    bool (*build_new) (LogBuilder *builder);
    const uint8_t * (*get_ptr) (LogBuilder *builder);
    size_t (*get_size) (LogBuilder *builder);
};

typedef struct Logger Logger;

struct Logger
{
    LogBuilder *builder;
    LogSubscriber *subs;
    size_t num_subs;
    Send *comm;
    bool cmd_enable;
    bool enabled;
};

void logger_init(Logger *log, LogBuilder *builder, LogSubscriber *subs, size_t num_subs, Send *comm);
bool logger_update(Logger *log);
void logger_enable(Logger *log, bool enable);
bool logger_retrieve(Logger *log);
