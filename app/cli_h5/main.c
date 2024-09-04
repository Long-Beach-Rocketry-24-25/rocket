
#include "FreeRTOS.h"
#include "task.h"

#include "bsp.h"

#include "i2c.h"
#include "usart.h"
#include "usart_cli.h"

/*-----------------------------------------------------------*/

void blink(int argc, char* argv[]);
void read_temp(int argc, char* argv[]);

Usart usart;
I2c i2c;

int main(void)
{

    BSP_Init(&usart, &i2c);

    Command commands[2] = { 
        {"Blink", blink, "Blinks LED."},
        {"Temp", read_temp, "Reads temperature."}
    };
    create_cli_task(&usart, commands, 2);

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    for( ; ; )
    {
    }

    return 0;
}

void blink(int argc, char* argv[])
{
    GPIOA->ODR ^= GPIO_ODR_OD5;
    cli_write("Blink - %d", !!(GPIOA->ODR & GPIO_ODR_OD5));
}

void read_temp(int argc, char* argv[])
{
    uint8_t data[2] = {0};
    i2c.set_target(&i2c, 0b1001000 << 1);
    i2c.read(&i2c, 0, data, 2);
    cli_write("Data: %x %x", data[0],  data[1]);
    
    int16_t temp = ((data[0] << 8) | data[1]) >> 4;
    int16_t max_12bit = 0xFFF;

    if (temp & (0x800))
    {
        temp = -((~(temp & max_12bit) & max_12bit) + 1);
    }
    
    // Convert digital reading to analog temperature (1-bit is equal to 0.0625 C)
    cli_write("Temp: %d", (int) ((float) temp / 16));
}