#pragma once
#include <stdbool.h>
#include <stdint.h>

#define TJ_SEND_BUF_SIZE 256U

typedef struct Send_protocol Send_protocol;

struct Send_protocol
{
    uint8_t size;
    bool (*format)(Send_protocol* self, uint8_t target, uint8_t data_len, const char * data);
    bool (*parse)();
    char message[TJ_SEND_BUF_SIZE];
    //char last_msg[TJ_SEND_BUF_SIZE];


};

bool format(Send_protocol* self, uint8_t target, uint8_t data_len, const char * data);
void send_protocol_init(Send_protocol* sender, bool (*format)(Send_protocol* self, uint8_t target, uint8_t data_len, const char * data));
//bool formated_return(uint8_t target, uint8_t data_len, char * data);