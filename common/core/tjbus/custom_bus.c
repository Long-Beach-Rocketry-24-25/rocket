#include "custom_bus.h"

void send_protocol_init(Bus* sender, uint8_t address)
{
    sender->address = address;
    sender->state = IDLE;
    sender->format = format;
    sender->read_byte = read_byte;
    sender->receive_index = 0;
    sender->package_size = TJ_SEND_BUF_SIZE;
    //sender->acknowledged = true;
}

bool format(Bus* self, uint8_t target, uint8_t data_len, const uint8_t* data)
{
    uint8_t index = 0;
    char* output = self->send_buffer;
    uint32_t sum = 0;
    output[index++] = START_TRANSMISSION;  // start byte
    sum += START_TRANSMISSION;
    output[index++] = target;
    sum += target;
    output[index++] = data_len;
    sum += data_len;
    for (uint8_t i = 0; i < data_len; i++)
    {
        output[index++] = data[i];
        sum += data[i];
    }
    output[index++] = sum % 256;  //checksum
    return true;
}

bool read_byte(Bus* self, uint8_t data)
{

    {
        switch (self->state)
        {
            case IDLE:
                if (data == NACK)
                {
                    self->state = ERROR;
                }
                else if (data == START_TRANSMISSION)
                {
                    self->state = READ_ADDRESS;
                    memset(self->receive_buffer, 0, TJ_SEND_BUF_SIZE);
                    self->sum = 0;
                    self->receive_index = 0;
                    self->package_size = 0;
                    self->receive_buffer[self->receive_index] = data;
                    self->sum += data;

                    self->receive_index += 1;
                }
                if (data == ACK)
                {
                    self->state = ACKNOWLEDGED;
                }
                break;
            case READ_ADDRESS:  // if address matches then continue if not go back to idle
                if (data == self->address)
                {
                    self->receive_buffer[self->receive_index] = data;
                    self->sum += data;
                    self->receive_index += 1;
                    self->state = READ_LEN;
                }
                else
                {
                    self->state = IDLE;
                }
                break;
            case READ_LEN:
                self->package_size = data;
                self->receive_buffer[self->receive_index] = data;
                self->sum += data;
                self->receive_index += 1;
                self->state = READ_DATA;
                break;
            case READ_DATA:
                if (self->receive_index == self->package_size + 2)
                {
                    self->state = VALIDATE;
                }
                self->receive_buffer[self->receive_index] = data;
                self->sum += data;
                self->receive_index += 1;

                break;
            case VALIDATE:
                self->sum = self->sum % 256;
                if (self->sum != data)
                {
                    self->state = ERROR;
                    self->receive_buffer[self->receive_index] = data;
                    self->sum += data;
                    self->receive_index += 1;
                }
                else
                {
                    self->state = FINISHED;
                    self->receive_buffer[self->receive_index] = data;
                    self->sum += data;
                    self->receive_index += 1;
                }

                break;
            case ERROR:
                break;
            default:
                break;
        }
    }
    return true;
}