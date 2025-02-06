
#include "FreeRTOS.h"
#include "task.h"

#include "cli_app_bsp.h"

#include "gpio.h"
#include "i2c.h"
#include "usart.h"

#include "debug_app.h"

/*-----------------------------------------------------------*/

Usart usart;
I2c i2c;
Gpio led_gpio;

int main(void)
{

    BSP_Init(&usart, &i2c, &led_gpio);

    DebugAppCreate(&usart, &i2c, &led_gpio);

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    while (1)
        ;

    return 0;
}
