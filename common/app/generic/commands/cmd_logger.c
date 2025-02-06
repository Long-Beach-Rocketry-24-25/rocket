
#include "cmd_logger.h"

#define NUM_LOG_COMMANDS 3

static Logger* logging;

static void cmd_logger_enable(int argc, char* argv[])
{
    logger_enable(logging, true);
}

static void cmd_logger_disable(int argc, char* argv[])
{
    logger_enable(logging, false);
}

static void cmd_logger_retrieve(int argc, char* argv[])
{
    logger_retrieve(logging);
}

static const Command log_cmds[NUM_LOG_COMMANDS] = {
    {"LogEnable", cmd_logger_enable, "Clears previous log and starts logging."},
    {"LogDisable", cmd_logger_disable, "Stops logging."},
    {"LogRetrieve", cmd_logger_retrieve,
     "Retrieves and outputs the saved log."}};

void cmd_logger_init(Cli* cli, Logger* logger)
{
    for (size_t i = 0; i < NUM_LOG_COMMANDS; ++i)
    {
        cli_register_command(cli, &log_cmds[i]);
    }
    logging = logger;
}
