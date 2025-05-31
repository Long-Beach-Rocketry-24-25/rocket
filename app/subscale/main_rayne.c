
#include "FreeRTOS.h"
#include "task.h"

#include "subscale_bsp.h"

#include "gpio.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"

#include "debug_app.h"
#include "subscale_app_rayne.h"

/*-----------------------------------------------------------*/

Usart usart;
Spi spi;
I2c i2c;
Gpio led_gpio;

int main(void)
{

    BSP_Init(&usart, &spi, &i2c, &led_gpio);
    SubscaleAppCreateRayne(&usart, &spi, &i2c, &led_gpio, SystemReset);
    uint8_t data[] = "test";

    usart.send(&usart, data, 4);
    usart.send(&usart, data, 4);

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    while (1);
    return 0;
}
