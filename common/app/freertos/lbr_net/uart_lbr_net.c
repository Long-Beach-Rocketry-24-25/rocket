#include "uart_lbr_net.h"

static Bus bus;
static Usart* comm;
static SemaphoreHandle_t task_semaphore = NULL;
static const pb_msgdesc_t* self_pb;  // idk if i need this

bool send(uint8_t target_address, void* message, void* message_schema)
{
    uint8_t pb_buffer[MAX_RECEIVE_BUF_SIZE - PACKET_HEADER_SIZE];
    size_t pb_length;
    bool status;
    pb_ostream_t stream = pb_ostream_from_buffer(pb_buffer, sizeof(pb_buffer));
    status = pb_encode(&stream, message_schema, message);
    pb_length = stream.bytes_written;
    if (!status)
    {
        return false;
    }
    uint8_t message_buffer[MAX_RECEIVE_BUF_SIZE];
    bus.pack(&bus, message_buffer, MAX_RECEIVE_BUF_SIZE, target_address,
             pb_buffer, pb_length);

    comm->send(&comm, &message_buffer, pb_length + PACKET_HEADER_SIZE);
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
