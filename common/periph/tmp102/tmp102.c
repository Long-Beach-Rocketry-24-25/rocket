
#include "tmp102.h"

static const max_addr = 3;
static const addr_base = 0x48;

bool Tmp102_Init(Tmp102 *dev, I2c *bus, uint8_t dev_addr)
{
    if (dev_addr <= max_addr)
    {
        dev->addr = (addr_base | dev_addr) << 1;
    }
    else
    {
        return false;
    }

    dev->bus = bus;
    dev->status = true;
    dev->get_temp_c = Tmp102_Get_Temp_C;

    return true;
}

float Tmp102_Get_Temp_C(Tmp102 *dev)
{
    uint8_t data[2] = {0};
    dev->bus->set_target(dev->bus, dev->addr);
    dev->status = dev->bus->read(dev->bus, 0, data, 2);
    
    int16_t temp = ((data[0] << 8) | data[1]) >> 4;
    int16_t max_12bit = 0xFFF;

    if (temp & (0x800))
    {
        temp = -((~(temp & max_12bit) & max_12bit) + 1);
    }

    return (float) temp / 16;
}