

/*
//send
uint8_t message_buffer[MAX_RECEIVE_BUF_SIZE];
uint8_t data[MAX_RECEIVE_BUF_SIZE];
uint8_t pb_size = cmd_message->get_size(cmd_message);
cmd_message->get_buf(cmd_message, data, MAX_RECEIVE_BUF_SIZE);
bus.pack(&bus, message_buffer, MAX_RECEIVE_BUF_SIZE, target_address, data,
         pb_size);

//receive
uint8_t size = bus.get_package_size(&bus);
uint8_t data[size];
cmd_message->parse(cmd_message, data, size);
*/