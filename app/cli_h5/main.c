
#include "FreeRTOS.h"
#include "task.h"

#include "bsp.h"

#include "i2c.h"
#include "usart.h"
#include "usart_cli.h"

#include "tmp102.h"

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

uint8_t char_to_hex(char data)
{
    if (data >= '0' && data <= '9')
    {
        return data - '0';
    }
    else if (data >= 'A' && data <= 'F')
    {
        return data - 'A' + 10;
    }
    else if (data >= 'a' && data <= 'f')
    {
        return data - 'a' + 10;
    }
}

bool is_hex(char data)
{
    return (data >= '0' && data <= '9') ||
           (data >= 'A' && data <= 'F');
}

void write_i2c(int argc, char* argv[])
{
    #define MAX_SIZE 16
    size_t count = 0;
    bool valid = true;
    uint8_t id = 0;
    uint16_t address = 0;
    uint8_t data[MAX_SIZE] = {0};

    for (int i = 1; i < argc && valid; ++i)
    {
        size_t count = 0;
        int j = 0;
        while (argv[i][j] != '\0')
        {
            if (is_hex(argv[i][j]))
            {
                if (i == 1)
                {
                    if (j < 2)
                    {
                        id <<= 4;
                        id |= char_to_hex(argv[i][j]);
                    }
                    else
                    {
                        valid = false;
                        break;
                    }
                }
                else if (i == 2)
                {
                    if (j < 4)
                    {
                        address <<= 4;
                        address |= char_to_hex(argv[i][j]);
                    }
                    else
                    {
                        valid = false;
                        break;
                    }
                }
                else
                {
                    data[i - 3] = char_to_hex(argv[i][j]);
                }
                j++;
            }
            else
            {
                valid = false;
                break;
            }
        }
        count++;

        /*
         * i >= ( (max_size - 1) + 2 ), because
         * address takes up 1 argument, id takes up 1.
         */
        if (i >= MAX_SIZE)
        {
            break;
        }
    }

    cli_write("Writing: %x %x %x %x...", id, address, data[0], data[1]);
    i2c.write(&i2c, address, data, count);
}

void read_i2c(int argc, char *argv[])
{
    #define MAX_SIZE 3
    bool valid = true;
    uint8_t id = 0;
    uint16_t address = 0;
    uint16_t size = 0;
    uint8_t data[MAX_SIZE] = {0};

    for (int i = 1; i < argc && valid; ++i)
    {
        int j = 0;
        while (argv[i][j] != '\0')
        {
            if (is_hex(argv[i][j]))
            {
                if (i == 1)
                {
                    if (j < 2)
                    {
                        id <<= 4;
                        id |= char_to_hex(argv[i][j]);
                    }
                    else
                    {
                        valid = false;
                        break;
                    }
                }
                else if (i == 2)
                {
                    if (j < 4)
                    {
                        address <<= 4;
                        address |= char_to_hex(argv[i][j]);
                    }
                    else
                    {
                        valid = false;
                        break;
                    }
                }
                else if (i == 3)
                {
                    size <<= 4;
                    size |= char_to_hex(argv[i][j]);
                }
                else
                {
                    valid = false;
                    break;
                }
                j++;
            }
            else
            {
                valid = false;
                break;
            }
        }

        /*
         * 3 args max.
         */
        if (i >= MAX_SIZE + 1)
        {
            break;
        }
    }

    cli_write("Reading: %x %x %x ", id, address, size);
    i2c.set_target(&i2c, id << 1);
    i2c.read(&i2c, address, data, size);
    cli_write("Received: %x %x", data[0], data[1]);
}