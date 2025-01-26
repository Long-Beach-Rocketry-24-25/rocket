
#include "FreeRTOS.h"
#include "task.h"

#include "subscale_bsp.h"

#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

#include "debug_app.h"
#include "subscale_app.h"

/*-----------------------------------------------------------*/

Usart usart;
Spi spi;
I2c i2c;
Gpio led_gpio;

int main(void)
{

    BSP_Init(&usart, &spi, &i2c, &led_gpio);
    SubscaleAppCreate(&usart, &spi, &i2c, &led_gpio);

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    while (1);
    return 0;
}
