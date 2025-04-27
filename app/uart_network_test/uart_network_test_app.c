#include "uart_network_test_app.h"

#define RECEIVER_ADDRESS 41
uint8_t data = 1;
size_t data_size = sizeof(data_size);
network_if interface;
static void loop_func(void)
{

    interface.send(RECEIVER_ADDRESS, &data, data_size);
}
void NetworkUartTest(Usart* usart)
{

    uart_lbr_init(ADDRESS, &interface, usart);
    create_main_loop(loop_func, 1);
}