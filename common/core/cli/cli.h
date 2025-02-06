/**
 * 
 * 
 */

#pragma once

#include "send.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MAX_CMD_LENGTH 64
#define MAX_RSP_LENGTH 64
#define MAX_ARGS 8

typedef void (*CommandHandler)(int argc, char* argv[]);

typedef struct
{
    const char* name;
    CommandHandler handler;
    const char* help;
} Command;

typedef struct Cli Cli;

struct Cli
{
    Send* comm;
    Command commands[MAX_ARGS];
    uint32_t num_commands;
};

void cli_init(Cli* cli, Send* comm);
Send* cli_get_sender(Cli* cli);
bool cli_process(Cli* cli, const char* message);
bool cli_register_command(Cli* cli, const Command* cmd);