/**
 * 
 * 
 */

#pragma once


#include "send.h"

#include "stdint.h"
#include "stdbool.h"
#include "string.h"


#define MAX_CMD_LENGTH 64
#define MAX_RSP_LENGTH 15
#define MAX_ARGS 8

typedef void (*CommandHandler)(int argc, char *argv[]);

typedef struct {
    const char *name;
    CommandHandler handler;
    const char *help;
} Command;

void cli_init(Send *comm);
bool cli_process(const char * message);
bool cli_register_command(const Command *cmd);