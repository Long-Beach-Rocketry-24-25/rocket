
#include "usart_hw_bsp.h"

#include "stm32f746xx.h"

static StPrivUsart st_cli_usart;
static StPrivUsart st_comm_usart;
static StGpioParams led_stgpio = {{0}, GPIOB_BASE, 0, {1, 0, 0, 0, 0}};
static StGpioParams txe_gpio = {{0}, GPIOC_BASE, 12, {1, 0, 0, 0, 0}};
static StGpioParams rxe_gpio = {{0}, GPIOD_BASE, 2, {1, 0, 0, 0, 0}};

// Sequential use of these, so using one is fine. Not thread safe.
static Timeout time;
static FrtTimerData frt;

static StGpioParams cli_uart_io1 = {{0},
                                    GPIOD_BASE,
                                    8,
                                    {ALT_FUNC, 0, 0, 0, 0x7}};  // USART3 AF 7
static StGpioParams cli_uart_io2 = {{0},
                                    GPIOD_BASE,
                                    9,
                                    {ALT_FUNC, 0, 0, 0, 0x7}};  // USART3 AF 7

static StGpioParams comm_uart_io1 = {{0},
                                     GPIOC_BASE,
                                     10,
                                     {ALT_FUNC, 0, 0, 0, 0x8}};  // UART4 AF 8
static StGpioParams comm_uart_io2 = {{0},
                                     GPIOC_BASE,
                                     11,
                                     {ALT_FUNC, 0, 0, 0, 0x8}};  // UART4 AF 8

static RingBuffer rb1;
static RingBuffer rb2;
static uint8_t arr1[UART_PIPE_BUF_SIZE] = {0};
static uint8_t arr2[UART_PIPE_BUF_SIZE] = {0};

Snx5176b rs485;
Usart* u3;
Usart* u4;

void BSP_Init(Usart* cli_usart, Usart* comm_usart, Gpio* led_gpio)
{

    __disable_irq();

    // LED GPIO
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    StGpioInit(led_gpio, &led_stgpio);
    StGpioConfig(led_gpio);

    // Single FreeRTOS timer
    frt_timer_init(&time, &frt, 100);

    // Cli USART3
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

    StGpioInit(&st_cli_usart.rx, &cli_uart_io1);
    StGpioInit(&st_cli_usart.tx, &cli_uart_io2);

    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(0, 6, 0));
    NVIC_EnableIRQ(USART3_IRQn);

    StUsartInit(cli_usart, &st_cli_usart, USART3_BASE, &time);
    StUsartConfig(cli_usart, SystemCoreClock, 115200);

    // Comm UART4
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    StGpioInit(&st_comm_usart.rx, &comm_uart_io1);
    StGpioInit(&st_comm_usart.tx, &comm_uart_io2);

    RCC->APB1ENR |= RCC_APB1ENR_UART4EN;

    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority(UART4_IRQn, NVIC_EncodePriority(0, 6, 0));
    NVIC_EnableIRQ(UART4_IRQn);

    StUsartInit(&rs485.usart, &st_comm_usart, UART4_BASE, &time);
    StUsartConfig(&rs485.usart, SystemCoreClock, 921600);

    ring_buffer_init(&rb1, arr1, UART_PIPE_BUF_SIZE);
    ring_buffer_init(&rb2, arr2, UART_PIPE_BUF_SIZE);

    StGpioInit(&rs485.txe, &txe_gpio);
    StGpioConfig(&rs485.txe);

    StGpioInit(&rs485.rxe, &rxe_gpio);
    StGpioConfig(&rs485.rxe);

    Snx5176bInit(comm_usart, &rs485);
    Snx5176bConfig(&rs485);

    u3 = cli_usart;
    u4 = comm_usart;

    __enable_irq();
}

void USART3_IRQHandler(void)
{
    UartPipeCallback(u3, u4, &rb1, UART_END_CHAR);
}

void UART4_IRQHandler(void)
{
    UartPipeCallback(u4, u3, &rb2, UART_END_CHAR);
}