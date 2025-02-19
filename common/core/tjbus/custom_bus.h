#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define TJ_SEND_BUF_SIZE 255U

typedef enum
{
    IDLE,
    ERROR,
    READ_LEN,
    READ_DATA,
    VALIDATE,
    FINISHED
} BusState;

typedef enum
{
    START_TRANSMISSION = '!',
    END_TRANSMISSON = '$',
    ACK = '+',
    NACK = '-'
} ControlFlags;

typedef struct Bus Bus;
//need a nack ack timeout and start command
struct Bus
{
    uint8_t package_size;
    BusState state;
    uint32_t sum;
    uint8_t receive_index;
    bool acknowledged;
    char send_buffer[TJ_SEND_BUF_SIZE];  // remove later
    char receive_buffer[TJ_SEND_BUF_SIZE];
    bool (*format)(Bus* self, uint8_t target, uint8_t data_len,
                   const char* data);
    bool (*read_byte)(Bus* self, uint8_t data);
    bool (*get_package_size)(void);
    //bool (*get_msg)(void);
};

bool format(Bus* self, uint8_t target, uint8_t data_len, const char* data);
bool read_byte(Bus* self, uint8_t data);
bool reset_receive(Bus* self);
bool get_package_size(void);
bool get_msg(void);

void send_protocol_init(Bus* sender);

//bool formated_return(uint8_t target, uint8_t data_len, char * data);