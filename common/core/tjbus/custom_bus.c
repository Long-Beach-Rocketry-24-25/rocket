#include "custom_bus.h"

void send_protocol_init(Bus* sender)
{
    sender->state = IDLE;
    sender->format = format;
    sender->read_byte = read_byte;
    sender->receive_index = 0;
    sender->package_size = TJ_SEND_BUF_SIZE;
    sender->acknowledged = true;
}

bool format(Bus* self, uint8_t target, uint8_t data_len, const char* data)
{
    //char output[data_len + 4]; // start + data + check sum + end;
    char* output = self->send_buffer;
    uint32_t sum = 0;
    output[0] = START_TRANSMISSION;  // start byte
    sum += START_TRANSMISSION;
    output[1] = target;
    sum += target;
    for (uint8_t i = 0; i < data_len; i++)
    {
        output[i + 2] = data[i];
        sum += data[i];
    }
    //output[data_len + 2] = END_TRANSMISSON;  //end byte
    //sum += END_TRANSMISSON;
    output[data_len + 2] = sum % 256;  //checksum
    return true;
}

bool read_byte(Bus* self, uint8_t data)
{
    if (data == NACK)
    {
        self->state = ERROR;
    }
    else
    {
        switch (self->state)
        {
            case IDLE:
                if (data == START_TRANSMISSION)
                {
                    self->state = READ_LEN;
                    memset(self->receive_buffer, 0, TJ_SEND_BUF_SIZE);
                    self->sum = 0;
                    self->receive_index = 0;
                    self->receive_buffer[self->receive_index] = data;
                    self->sum += data;
                    self->receive_index += 1;
                }
                if (data == ACK)
                {
                    self->acknowledged = true;
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
                if (self->receive_index == self->package_size + 1)
                {
                    self->state = VALIDATE;
                }
                self->receive_buffer[self->receive_index] = data;
                self->sum += data;
                self->receive_index += 1;
                break;
            case VALIDATE:
                if (self->sum != data)
                {
                    self->state = ERROR;
                }
                self->state = FINISHED;
                break;
            case ERROR:
                break;
            default:
                break;
        }
    }
    return true;
}