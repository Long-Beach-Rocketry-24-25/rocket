
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

uint8_t tx[6] = {0x9F, 0, 0, 0, 0, 0};
uint8_t db[5] = {0xDE, 0xCA, 0xFE, 0xCA, 0xFE};
uint8_t rx[6] = {0};

int main(void)
{

    BSP_Init(&usart, &spi, &i2c, &led_gpio);

    spi.cs.select(&spi.cs);
    spi.send(&spi, tx, 1);
    spi.read(&spi, rx, 3);
    W25qInit(&flash, &spi);

    flash.read(&flash, 0, rx, 6);
    if (rx[0] != 0xDE)
    {
        flash.page_write(&flash, 0, db, 5);
    }
    else
    {
        flash.read(&flash, 0, rx, 5);
        volatile s = flash.erase_sector(&flash, 0);
    }
    volatile c = flash.read(&flash, 0, rx, 6);

    // DebugAppCreate(&usart, &i2c, &led_gpio);

    // /* Start the scheduler to start the tasks executing. */
    // vTaskStartScheduler();

    while(1)
    {
        spi.cs.select(&spi.cs);
        spi.transact(&spi, tx, rx, 6);
        spi.cs.deselect(&spi.cs);
    }

    return 0;
}
