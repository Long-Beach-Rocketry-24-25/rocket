#include "tj_send.h"

void send_protocol_init(Send_protocol* sender, bool (*format)(Send_protocol* self, uint8_t target, uint8_t data_len, const char * data)){
    sender->format = format;

}


bool format(Send_protocol* self, uint8_t target, uint8_t data_len, const char * data){
    //char output[data_len + 4]; // start + data + check sum + end;
    char * output = self->message;
    uint32_t sum = 0;
    output[0] = '!'; // start byte
    sum += 33;
    output[1] = target;
    sum += target;
    for (uint8_t i = 0; i < data_len; i++){
        output[i+2] = data[i];
        sum += data[i];
    }
    output[data_len + 2] = '$'; //end byte
    sum += 36;
    output[data_len + 3] =  sum % 256; //checksum
    return true;
}