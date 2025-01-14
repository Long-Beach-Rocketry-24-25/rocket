#include "ads111x.h"

static const uint8_t max_addr = 3;
static const uint8_t addr_base = 0x48;

bool Ads111x_Init(Ads111x *dev, I2c *bus, uint8_t dev_addr)
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

    dev->take_sample = Ads111x_Sample_Channel;
    dev->start_sample = Ads111x_Start_Sample;
    dev->read_sample = Ads111x_Read_Sample;

    return true;
}

void Ads111x_Config(Ads111x *dev, uint8_t gain, uint8_t mode, uint8_t datarate)
{
    dev->gain = gain;
    dev->mode = mode;
    dev->datarate = datarate;
}

static void Ads111x_Wait_Complete(Ads111x * dev)
{
    /*
     * Wait for adc to be no longer in conversion.
     */
    uint8_t rxdata[2] = {0};
    while (!(rxdata[0] & ADS_CONFIG_OS))
    {
    dev->status = dev->bus->read(dev->bus, ADS_REG_CONFIG_PTR,
                                 rxdata, ADS_CONFIG_BUFFER_SIZE);
    }
}

void Ads111x_Start_Sample(Ads111x * dev, uint8_t channel)
{
    /*
     * Store configs in buffer to send.
     */
    uint8_t configs[2] = {0};
    configs[1] = dev->datarate;
    configs[0] = ADS_CONFIG_UPPER_BASE | ADS_CONFIG_OS | dev->mode
                | dev->gain | channel;

    /*
     * Set I2C ID.
     */
    dev->status = dev->bus->set_target(dev->bus, dev->addr);

    /*
     * Configure and start conversion.
     */
    dev->status = dev->bus->write(dev->bus, ADS_REG_CONFIG_PTR,
                                  configs, ADS_CONFIG_BUFFER_SIZE);
}

int16_t Ads111x_Read_Sample(Ads111x *dev)
{
    /*
     * Set I2C ID.
     */
    dev->status = dev->bus->set_target(dev->bus, dev->addr);

    /*
     * Read last conversion.
     */
    uint8_t rxdata[2] = {0};
    dev->status = dev->bus->read(dev->bus, ADS_REG_CONVERSION_PTR,
                                 rxdata, ADS_CONFIG_BUFFER_SIZE);

    return (int16_t) ((rxdata[0] << 8) | rxdata[1]);
}

int16_t Ads111x_Sample_Channel(Ads111x * dev, uint8_t channel)
{
    Ads111x_Start_Sample(dev, channel);
    Ads111x_Wait_Complete(dev);
    return Ads111x_Read_Sample(dev);
}