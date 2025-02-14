#include "tj_send.h"

void send_protocol_init(Send_protocol* sender, bool (*write_func)(uint8_t target, uint8_t data_len, char * data)){
    sender->write_command = write_func;
}


