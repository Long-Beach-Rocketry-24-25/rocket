#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "cmd_message.h"
typedef struct network_if network_if;
struct network_if
{
    bool (*send)(uint8_t target_address, cmd_message* cmd_message);
    bool (*receive)(cmd_message* cmd_message);
};