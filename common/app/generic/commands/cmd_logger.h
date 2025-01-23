
#pragma once

#include "cli.h"
#include "logging.h"

/**
 * logging [ enable | disable | retrieve]
 */

void cmd_logger_init(Send *sender, Logger *logger);
void cmd_logger(int argc, char* argv[]);
void cmd_logger_enable(int argc, char* argv[]);
void cmd_logger_disable(int argc, char* argv[]);
void cmd_logger_retrieve(int argc, char* argv[]);
