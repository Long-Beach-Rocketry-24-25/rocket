
#include "FreeRTOS.h"
#include "task.h"

#include "gpio_test_bsp.h"

#include "main_loop.h"

#include "gpio.h"

/*-----------------------------------------------------------*/

Gpio led_gpio;

void gpio_test(void)
{
    led_gpio.toggle(&led_gpio);
}

int main(void)
{

    BSP_Init(&led_gpio);

    create_main_loop(gpio_test, 1);

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    while (1);

    return 0;
}
