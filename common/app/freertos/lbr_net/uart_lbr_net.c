#include "uart_lbr_net.h"

void uart_lbr_net_init(uart_lbr_net* uart_lbr_net, Send* sender,
                       uint8_t address)
{
    Bus bus;
    lbr_net_node_init(&bus, address);
    uart_lbr_net->bus = bus;
    uart_lbr_net->send_data = sender->write_str;
    //send
    //reicves
    //calback
}