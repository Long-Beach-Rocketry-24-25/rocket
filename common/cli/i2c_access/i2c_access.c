
#include "i2c_access.h"

static I2c i2c;

void init_i2c_access(I2c *i2c_dev)
{
    i2c = *i2c_dev;
}

void write_i2c(int argc, char* argv[])
{
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

