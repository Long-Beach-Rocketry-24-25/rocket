#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define MAX_RECEIVE_BUF_SIZE 255U
#define CHECKSUM_SIZE 1
#define CHECKSUM_BITS 256
#define START_BYTE_SIZE 1
#define ADDRESS_SIZE 1
#define DATA_LEN_SIZE 1
#define PACKET_HEADER_SIZE \
    (CHECKSUM_SIZE + START_BYTE_SIZE + ADDRESS_SIZE + DATA_LEN_SIZE)

typedef enum
{
    IDLE,
    ERROR,
    ACKNOWLEDGED,
    READ_ADDRESS,
    READ_LEN,
    READ_DATA,
    VALIDATE,
    FINISHED
} BusState;

typedef enum
{
    START_TRANSMISSION = '!',
    ACK = '+',
    NACK = '-'
} ControlFlags;

/**
 * @brief This is the bus struct which can format and parse messages as well as store a state
 */
typedef struct Bus Bus;
struct Bus
{
    /* max data package size*/
    uint8_t package_size;
    /* bus state */
    BusState state;
    /* current checksum */
    uint32_t sum;
    /* what index of the receive buffer we are on */
    uint8_t receive_index;
    /*this buses address*/
    uint8_t address;
    char receive_buffer[MAX_RECEIVE_BUF_SIZE];
    bool (*format)(Bus* self, uint8_t* buffer, uint16_t buffer_size,
                   uint8_t target, const uint8_t* data, uint8_t data_size);
    bool (*read_byte)(Bus* self, uint8_t data);
    uint8_t (*get_package_size)(Bus* self);
    bool (*receive_flush)(Bus* self, uint8_t* buffer);
};

/**
 * @brief Converts data into a formatted message and clears/writes to a buffer.
 * 
 * @param self A pointer the current sending bus struct.
 * @param buffer A pointer a buffer that we will store the output in.
 * @param buffer_size The size of the output buffer.
 * @param target The address of what we want to use in the message.
 * @param data A pointer to the data buffer.
 * @param data_size The size of the data buffer.
 * 
 */
bool format(Bus* self, uint8_t* buffer, uint16_t buffer_size, uint8_t target,
            const uint8_t* data, uint8_t data_size);

/**
 * @brief Reads a byte stores in receive buffer and updates the bus state.
 * @param self A pointer to the bus struct. 
 * @param data The byte that is going to be read.
 */
bool read_byte(Bus* self, uint8_t data);

/**
 * @brief Clears and sends data from receive buffer to another buffer
 * @param self A pointer to the bus struct.
 * @param buffer A pointer to the destination buffer    
 */
bool receive_flush(Bus* self, uint8_t* buffer);

/**
 * @brief Return the package size
 * @param self A pointer to the bus struct.
 */
uint8_t get_package_size(Bus* self);

/**
 * @brief Initialized bus struct
 * @param self A pointer to the bus struct.
 * @param address the address we want to assign to the struct
 * 
 * sets receive and package size to 0.
 * sets state to IDLE
 * initializes all functions
 */
void send_protocol_init(Bus* sender, uint8_t address);
