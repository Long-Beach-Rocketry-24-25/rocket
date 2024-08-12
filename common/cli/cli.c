/**
 * 
 * 
 */

#include "cli.h"

static StrComm *comm;
static Command commands[MAX_ARGS];
static uint32_t num_commands = 0;

void cli_init(StrComm *driver) {
    comm = driver;
}

bool cli_register_command(const Command *cmd) {
    if (num_commands < MAX_ARGS) {
        commands[num_commands++] = *cmd;
    }
}


static void parse_command(char *input, int *argc, char *argv[]) {
    *argc = 0;
    char *token = strtok(input, " ");
    while (token != NULL && *argc < MAX_ARGS) {
        argv[(*argc)++] = token;
        token = strtok(NULL, " ");
    }
}

void cli_process(void) {
    char input[MAX_CMD_LENGTH];
    bool success = comm->read_str(input, MAX_CMD_LENGTH);
    
    if (success) {
        int argc;
        char *argv[MAX_ARGS];
        parse_command(input, &argc, argv);
        
        if (argc > 0) {
            for (int i = 0; i < num_commands; i++) {
                if (strcmp(argv[0], commands[i].name) == 0) {
                    commands[i].handler(argc, argv);
                    return;
                }
            }
            
            /*
             * Unknown command parsed.
             */
            char response[] = "Unknown command!\n";
            comm->write_str(response, strlen(response));
        }
    }
}

