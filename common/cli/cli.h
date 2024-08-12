/**
 * 
 * 
 */

#pragma once


#include "str_comm.h"

#include "FreeRTOS.h"
#include "task.h"

#include "stdint.h"
#include "stdbool.h"
#include "string.h"


#define MAX_CMD_LENGTH 64
#define MAX_ARGS 8

typedef void (*CommandHandler)(int argc, char *argv[]);

typedef struct {
    const char *name;
    CommandHandler handler;
    const char *help;
} Command;

void cli_init(StrComm *comm);
void cli_process(void);
bool cli_register_command(const Command *cmd);