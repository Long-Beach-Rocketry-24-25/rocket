
#include "FreeRTOS.h"
#include "task.h"

#include "bsp.h"

#include "i2c.h"
#include "i2c_access.h"
#include "usart.h"
#include "usart_cli.h"
#include "gpio.h"

#include "tmp102.h"
#include "bno055.h"
#include "bmp390.h"

#include "blink.h"
#include "read_tmp102.h"
#include "read_bmp390.h"
#include "read_bno055.h"

#include "gnc.h"

/*-----------------------------------------------------------*/

Usart usart;
I2c i2c;
Gpio led_gpio;

Tmp102 tmp;
Bno055 bno;
Bmp390 bmp;

int main(void)
{

    BSP_Init(&usart, &i2c, &led_gpio);

    Tmp102_Init(&tmp, &i2c, TMP102_ADDR_GND);

    Bno055_Init(&bno, &i2c, BNO055_DEV_ADDR);

    Bmp390_Init(&bmp, &i2c, BMP390_DEV_ADDR);

    init_i2c_access(&i2c);

    init_blink(&led_gpio);
    init_read_tmp102(&tmp);
    init_read_bno055(&bno);
    init_read_bmp390(&bmp);

    Command commands[6] = { 
        {"Blink", blink, "Blinks LED."},
        {"Temp", read_tmp102, "Reads temperature."},
        {"Imu", read_bno055, "Reads IMU accel/gyro."},
        {"Baro", read_bmp390, "Reads Barometer Pressure."},
        {"IWrite", write_i2c, "Writes I2c."},
        {"IRead", read_i2c, "Reads I2c."}
    };
    create_cli_task(&usart, commands, 6);
    cli_write("USING HW: STM32H503");

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    for( ; ; )
    {
    }

    return 0;
}
