
#include "bmp390.h"


static inline uint16_t combine_16u(uint8_t msb, uint8_t lsb)
{
    return (uint16_t) msb << 8 | lsb;
}

static inline int16_t combine_16i(int16_t msb, int16_t lsb)
{
    return (int16_t) msb << 8 | lsb;
}

void Bmp390_Init(Bmp390* dev, I2c* i2c, uint8_t addr)
{
    dev->bus = i2c;
    dev->addr = addr;
    dev->status = true;
    dev->get_pressure_pa = Bmp390_Get_Pressure;
    dev->get_temp_c = Bmp390_Get_Temp_C;
}

static void Bmp390_Get_Calib(Bmp390* dev)
{
    uint8_t data[BMP390_CALIB_REG_LEN] = {0};
    dev->status = dev->bus->set_target(dev->bus, dev->addr);
    dev->status = dev->bus->read(dev->bus, BMP390_CALIB_REG_START, data, BMP390_CALIB_REG_LEN);

    dev->cal.t1 = combine_16u(data[1], data[0]);
    dev->cal.t2 = combine_16u(data[3], data[2]);
    dev->cal.t3 = (int8_t) data[4];
    dev->cal.p1 = combine_16i(data[6], data[5]);
    dev->cal.p2 = combine_16i(data[8], data[7]);
    dev->cal.p3 = (int8_t) data[9];
    dev->cal.p4 = (int8_t) data[10];
    dev->cal.p5 = combine_16u(data[12], data[11]);
    dev->cal.p6 = combine_16u(data[14], data[13]);
    dev->cal.p7 = (int8_t) data[15];
    dev->cal.p8 = (int8_t) data[16];
    dev->cal.p9 = combine_16i(data[18], data[17]);
    dev->cal.p10 = (int8_t) data[19];
    dev->cal.p11 = (int8_t) data[20];
}

void Bmp390_Config(Bmp390* dev)
{
    Bmp390_Get_Calib(dev);
    uint8_t conf = BMP390_PWR_CTRL_MODE | BMP390_PWR_CTRL_PRESS_EN | BMP390_PWR_CTRL_TEMP_EN;
    dev->status = dev->bus->write(dev->bus, BMP390_PWR_CTRL_REG, &conf, 1);
}

static int64_t Bmp390_Compensate_Temperature(Bmp390 *dev, uint32_t data)
{ 
    uint64_t partial_data1;
    uint64_t partial_data2;
    uint64_t partial_data3;
    int64_t partial_data4;
    int64_t partial_data5;
    int64_t partial_data6;
    int64_t comp_temp;

    /* calculate compensate temperature */
    partial_data1 = (uint64_t)(data - (256 * (uint64_t)(dev->cal.t1)));
    partial_data2 = (uint64_t)(dev->cal.t2 * partial_data1);
    partial_data3 = (uint64_t)(partial_data1 * partial_data1);
    partial_data4 = (int64_t)(((int64_t)partial_data3) * ((int64_t)dev->cal.t3));
    partial_data5 = ((int64_t)(((int64_t)partial_data2) * 262144) + (int64_t)partial_data4);
    partial_data6 = (int64_t)(((int64_t)partial_data5) / 4294967296U);
    dev->cal.t_fine = partial_data6;
    comp_temp = (int64_t)((partial_data6 * 25)  / 16384);
    
    return comp_temp;
}

static int64_t Bmp390_Compensate_Pressure(Bmp390* dev, uint32_t data)
{
    int64_t partial_data1;
    int64_t partial_data2;
    int64_t partial_data3;
    int64_t partial_data4;
    int64_t partial_data5;
    int64_t partial_data6;
    int64_t offset;
    int64_t sensitivity;
    uint64_t comp_press;

    /* calculate compensate pressure */
    partial_data1 = dev->cal.t_fine * dev->cal.t_fine;
    partial_data2 = partial_data1 / 64;
    partial_data3 = (partial_data2 * dev->cal.t_fine) / 256;
    partial_data4 = (dev->cal.p8 * partial_data3) / 32;
    partial_data5 = (dev->cal.p7 * partial_data1) * 16;
    partial_data6 = (dev->cal.p6 * dev->cal.t_fine) * 4194304;
    offset = (int64_t)((int64_t)(dev->cal.p5) * (int64_t)140737488355328U) + partial_data4 + partial_data5 + partial_data6;
    partial_data2 = (((int64_t)dev->cal.p4) * partial_data3) / 32;
    partial_data4 = (dev->cal.p3 * partial_data1) * 4;
    partial_data5 = ((int64_t)(dev->cal.p2) - 16384) * ((int64_t)dev->cal.t_fine) * 2097152;
    sensitivity = (((int64_t)(dev->cal.p1) - 16384) * (int64_t)70368744177664U) + partial_data2 + partial_data4 + partial_data5;
    partial_data1 = (sensitivity / 16777216) * data;
    partial_data2 = (int64_t)(dev->cal.p10) * (int64_t)(dev->cal.t_fine);
    partial_data3 = partial_data2 + (65536 * (int64_t)(dev->cal.p9));
    partial_data4 = (partial_data3 * data) / 8192;
    partial_data5 = (partial_data4 * data) / 512;
    partial_data6 = (int64_t)((uint64_t)data * (uint64_t)data);
    partial_data2 = ((int64_t)(dev->cal.p11) * (int64_t)(partial_data6)) / 65536;
    partial_data3 = (partial_data2 * data) / 128;
    partial_data4 = (offset / 4) + partial_data1 + partial_data5 + partial_data3;
    comp_press = (((uint64_t)partial_data4 * 25) / (uint64_t)1099511627776U);

    return comp_press;
}

static void Bmp390_Get_Data(Bmp390* dev)
{
    uint8_t data[6] = {0};
    dev->status = dev->bus->set_target(dev->bus, dev->addr);
    dev->status = dev->bus->read(dev->bus, BMP390_PRESS_DAT_REG_START, data, 3);
    dev->status = dev->bus->read(dev->bus, BMP390_TEMP_DAT_REG_START, &data[3], 3);

    uint32_t press = (uint32_t) data[2] << 16 | (uint32_t) data[1] << 8 | data[0];
    uint32_t temp = (uint32_t) data[5] << 16 | (uint32_t) data[4] << 8 | data[3];

    dev->last_temp = Bmp390_Compensate_Temperature(dev, temp) / 100.0;
    dev->last_press = Bmp390_Compensate_Pressure(dev, press) / 100.0;
}

float Bmp390_Get_Pressure(Bmp390* dev)
{
    Bmp390_Get_Data(dev);
    return dev->last_press;
}

float Bmp390_Get_Temp_C(Bmp390* dev)
{
    Bmp390_Get_Data(dev);
    return dev->last_temp;
}