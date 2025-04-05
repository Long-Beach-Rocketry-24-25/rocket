#include "uart_lbr_net.h"

static Bus bus;
static Usart* comm;
static SemaphoreHandle_t task_semaphore = NULL;

bool send(uint8_t* buff, size_t size)
{
    if (size > MAX_RECEIVE_BUF_SIZE - CHECKSUM_SIZE - START_BYTE_SIZE)
    {
        return false;
    }
    for (int i = 0; i < size; i++)
    {
        comm->send(&comm, &buff, size);
    }
    return true;
}

void lbr_net_process_task()
{
    const TickType_t max_block_time = pdMS_TO_TICKS(UINT32_MAX);
    task_semaphore = xSemaphoreCreateBinary();
    while (1)
    {
        if (xSemaphoreTake(task_semaphore, max_block_time) == pdTRUE)
        {
                }
    }
}

void usart_rx_callback()
{
    uint8_t data = 0;
    if (comm->recv(comm, &data, 1))
    {
        BaseType_t higher_prio_task_woken = pdFALSE;
        bus.read_byte(&bus, bus.receive_buffer);
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        if (bus.state = FINISHED)
        {
            if (task_semaphore == NULL)
            {
                return;
            }
            else
            {
                xSemaphoreGiveFromISR(task_semaphore, &higher_prio_task_woken);
            }
        }
        portYIELD_FROM_ISR(higher_prio_task_woken);
    }
}

bool receive(uint8_t* buff, size_t size)
{
}

void uart_lbr_init(uint8_t address, net_if* net_if, Usart* usart)
{
    lbr_net_node_init(&bus, address);
    comm = usart;
    net_if->receive = &receive;
    net_if->send = &send;
}
