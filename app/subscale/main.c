
#include "FreeRTOS.h"
#include "task.h"

#include "subscale_bsp.h"

#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

#include "debug_app.h"

/*-----------------------------------------------------------*/

Usart usart;
Spi spi;
I2c i2c;
Gpio led_gpio;

int main(void)
{

    BSP_Init(&usart, &spi, &i2c, &led_gpio);

    uint8_t tx[6] = {0x9F, 0, 0, 0, 0, 0};
    uint8_t rx[6] = {0};

    // DebugAppCreate(&usart, &i2c, &led_gpio);

    // /* Start the scheduler to start the tasks executing. */
    // vTaskStartScheduler();

    while(1)
    {
        volatile bool success = spi.transact(&spi, tx, rx, 6);
    }

    return 0;
}
