
#include "FreeRTOS.h"
#include "task.h"

#include "subscale_bsp.h"

#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

#include "w25q.h"

#include "debug_app.h"

/*-----------------------------------------------------------*/

Usart usart;
Spi spi;
I2c i2c;
Gpio led_gpio;

W25q flash;

int main(void)
{

    BSP_Init(&usart, &spi, &i2c, &led_gpio);

    W25qInit(&flash, &spi, 0xFFFFFF);


    while (1);

    return 0;
}
