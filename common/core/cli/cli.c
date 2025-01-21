/**
 * 
 * 
 */

#include "cli.h"

void cli_init(Cli *cli, Send *driver)
{
    cli->comm = driver;
    cli->num_commands = 0;
}

Send *cli_get_sender(Cli *cli)
{
    return cli->comm;
}

bool cli_register_command(Cli *cli, const Command *cmd)
{
    if (cli->num_commands < MAX_ARGS)
    {
        cli->commands[cli->num_commands++] = *cmd;
        return true;
    }
    return false;
}

static void parse_command(char *input, int *argc, char *argv[]) {
    *argc = 0;
    char *token = strtok(input, " ");
    while (token != NULL && *argc < MAX_ARGS)
    {
        argv[(*argc)++] = token;
        token = strtok(NULL, " ");
    }
}

static void help(Cli *cli)
{
    cli->comm->fwrite(cli->comm, "Available commands:");
    for (int i = 0; i < cli->num_commands; i++)
    {
        cli->comm->fwrite(cli->comm, "%s - %s", cli->commands[i].name, cli->commands[i].help);
    }
}

bool cli_process(Cli *cli, const char * message)
{
    char input[MAX_CMD_LENGTH];
    bool found = false;
    for (int i = 0; i < MAX_CMD_LENGTH; ++i)
    {
        input[i] = message[i];
        if (message[i] == '\0')
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        return false;
    }

    int argc;
    char *argv[MAX_ARGS];
    parse_command(input, &argc, argv);

    if (argc > 0) {
        for (int i = 0; i < cli->num_commands; i++)
        {
            if (strcmp(argv[0], cli->commands[i].name) == 0)
            {
                cli->commands[i].handler(argc, argv);
                return true;
            }
        }

        if (strcmp(argv[0], "help") == 0)
        {
            help(cli);
            return true;
        } 
        
        /*
         * Unknown command parsed.
         */
        cli->comm->fwrite(cli->comm, "Unknown command!");
        return false;
    }

    return true;
}
