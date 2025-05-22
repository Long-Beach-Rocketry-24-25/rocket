
#include "FreeRTOS.h"
#include "task.h"

#include "flight_computer_bsp.h"

#include "gpio.h"
#include "i2c.h"
#include "usart.h"

#include "debug_app.h"
#include "subscale_app.h"

/*-----------------------------------------------------------*/

Usart usart;
I2c i2c;
Spi spi;
Gpio red_led;
Gpio green_led;
Gpio blue_led;

int main(void)
{

    BSP_Init(&usart, &i2c, &spi, &red_led, &green_led, &blue_led);

    SubscaleAppCreate(&usart, &spi, &i2c, &blue_led, SystemReset);
    ConfigIrq();

    red_led.set(&red_led, true);
    green_led.set(&green_led, true);
    blue_led.set(&blue_led, true);

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    while (1);

    return 0;
}
