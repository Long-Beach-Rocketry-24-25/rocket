#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct Send_protocol Send_protocol;

struct Send_protocol
{
    bool (*write_command)(uint8_t target, uint8_t data_len, char * data);
};


void send_protocol_init(Send_protocol* sender, bool (*write_func)(uint8_t target, uint8_t data_len, char * data));

