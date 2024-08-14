/**
 * 
 * 
 */

#include "cli.h"

static Send *comm;
static Command commands[MAX_ARGS];
static uint32_t num_commands = 0;

void cli_init(Send *driver)
{
    comm = driver;
}

bool cli_write(const char *data, ...)
{
    char buf[MAX_RSP_LENGTH];
    va_list vl;
    va_start(vl, data);
    vsprintf(buf, data, vl);
    va_end(vl);

    buf[MAX_RSP_LENGTH - 1] = '\0';

    return comm->write_str(buf); 
}

bool cli_register_command(const Command *cmd)
{
    if (num_commands < MAX_ARGS) {
        commands[num_commands++] = *cmd;
        return true;
    }
    return false;
}

static void parse_command(char *input, int *argc, char *argv[]) {
    *argc = 0;
    char *token = strtok(input, " ");
    while (token != NULL && *argc < MAX_ARGS) {
        argv[(*argc)++] = token;
        token = strtok(NULL, " ");
    }
}

bool cli_process(const char * message)
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
        for (int i = 0; i < num_commands; i++) {
            if (strcmp(argv[0], commands[i].name) == 0) {
                commands[i].handler(argc, argv);
                return true;
            }
        }
        
        /*
         * Unknown command parsed.
         */
        comm->write_str("Unknown command!");
        return false;
    }
}

