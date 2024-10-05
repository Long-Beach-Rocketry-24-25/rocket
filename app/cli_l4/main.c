
#include "FreeRTOS.h"
#include "task.h"

#include "bsp.h"

#include "i2c.h"
#include "i2c_access.h"
#include "usart.h"
#include "usart_cli.h"
#include "gpio.h"

#include "tmp102.h"

#include "gnc.h"

/*-----------------------------------------------------------*/

void blink(int argc, char* argv[]);
void read_temp(int argc, char* argv[]);
void read_imu(int argc, char* argv[]);
void read_baro(int argc, char* argv[]);

Usart usart;
I2c i2c;
Gpio led_gpio;

Tmp102 tmp;

int main(void)
{

    BSP_Init(&usart, &i2c, &led_gpio);
    led_gpio.set(&led_gpio, true);

    Command commands[6] = { 
        {"Blink", blink, "Blinks LED."},
        {"Temp", read_temp, "Reads temperature."},
        {"IWrite", write_i2c, "Writes I2c."},
        {"IRead", read_i2c, "Reads I2c."}
    };
    create_cli_task(&usart, commands, 6);

    Tmp102_Init(&tmp, &i2c, TMP102_ADDR_GND);

    init_i2c_access(&i2c);

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    for( ; ; )
    {
    }

    return 0;
}

void blink(int argc, char* argv[])
{
    cli_write("Blink - %d", led_gpio.toggle(&led_gpio));
}

void read_temp(int argc, char* argv[])
{
    float x = tmp.get_temp_c(&tmp);
    cli_write("Temp: %fC", x);
}
