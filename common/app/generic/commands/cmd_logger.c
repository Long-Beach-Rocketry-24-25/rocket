
#include "cmd_logger.h"

#define NUM_COMMANDS 3

static Send *send;
static Cli *cli;
static Logger *logging;

static const Command log_cmds[NUM_COMMANDS] =
{
    {"enable", cmd_logger_enable, "Clears previous log and starts logging."},
    {"disable", cmd_logger_disable, "Stops logging."},
    {"retrieve", cmd_logger_retrieve, "Retrieves and outputs the saved log."}
};

void cmd_logger_init(Send *sender, Logger *logger)
{
    cli_init(cli, send);
    for (size_t i = 0; i < NUM_COMMANDS; ++i)
    {
        cli_register_command(cli, &log_cmds[i]);
    }
    send = sender;
    logging = logger;
}

void cmd_logger(int argc, char* argv[])
{
    if (argc > 0)
    {
        cli_process(cli, argv[0]);
    }   
}

void cmd_logger_enable(int argc, char* argv[])
{
    logger_enable(logging, true);
}

void cmd_logger_disable(int argc, char* argv[])
{
    logger_enable(logging, false);
}

void cmd_logger_retrieve(int argc, char* argv[])
{
    logger_retrieve(logging);
}