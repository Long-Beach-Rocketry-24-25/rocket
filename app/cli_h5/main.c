
#include "FreeRTOS.h"
#include "task.h"

#include "bsp.h"

#include "i2c.h"
#include "usart.h"
#include "usart_cli.h"

#include "tmp102.h"

#include "string_hex.h"

/*-----------------------------------------------------------*/

void blink(int argc, char* argv[]);
void read_temp(int argc, char* argv[]);
void write_i2c(int argc, char* argv[]);
void read_i2c(int argc, char* argv[]);

Usart usart;
I2c i2c;

Tmp102 tmp;

int main(void)
{

    BSP_Init(&usart, &i2c);

    Tmp102_Init(&tmp, &i2c, TMP102_ADDR_GND);

    Command commands[4] = { 
        {"Blink", blink, "Blinks LED."},
        {"Temp", read_temp, "Reads temperature."},
        {"IWrite", write_i2c, "Writes I2c."},
        {"IRead", read_i2c, "Reads I2c."}
    };
    create_cli_task(&usart, commands, 4);

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
    float x = tmp.get_temp_c(&tmp);
    cli_write("Temp: %dC", (int) x);
}

void write_i2c(int argc, char* argv[])
{
    #define MAX_I2CW_SIZE 16
    size_t count = 0;
    bool valid = true;
    uint8_t id = 0;
    uint16_t address = 0;
    uint8_t data[MAX_I2CW_SIZE] = {0};

    for (int i = 1; i < argc && valid; ++i)
    {
        if (i == 1)
        {
            id = str_to_8(argv[i]);
        }
        else if (i == 2)
        {
            address = str_to_16(argv[i]);
        }
        else
        {
            data[i - 3] = str_to_8(argv[i]);
            count++;
        }

        /*
         * i >= ( (max_size - 1) + 2 ), because
         * address takes up 1 argument, id takes up 1.
         */
        if (i >= MAX_I2CW_SIZE)
        {
            break;
        }
    }

    cli_write("Writing: %x %x %x %x... s%d", id, address, data[0], data[1], count);
    i2c.set_target(&i2c, id << 1);
    i2c.write(&i2c, address, data, count);
}

void read_i2c(int argc, char *argv[])
{
    #define MAX_I2CR_SIZE 3
    bool valid = true;
    uint8_t id = 0;
    uint16_t address = 0;
    uint16_t size = 0;
    uint8_t data[MAX_I2CR_SIZE] = {0};

    for (int i = 1; i < argc && valid; ++i)
    {
        if (i == 1)
        {
            id = str_to_8(argv[i]);
        }
        else if (i == 2)
        {  
            address = str_to_16(argv[i]);
        }
        else
        {
            size = str_to_16(argv[i]);
        }

        /*
         * 3 args max.
         */
        if (i >= MAX_I2CR_SIZE + 1)
        {
            break;
        }
    }

    cli_write("Reading: %x %x %x ", id, address, size);
    i2c.set_target(&i2c, id << 1);
    i2c.read(&i2c, address, data, size);
    cli_write("Received: %x %x", data[0], data[1]);
}