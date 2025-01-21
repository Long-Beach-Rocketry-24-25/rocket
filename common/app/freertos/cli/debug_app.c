

#include "debug_app.h"

static Cli cli;
static Tmp102 tmp;
static Bno055 bno;
static Bmp390 bmp;

void DebugAppCreate(Usart *usart, I2c *i2c, Gpio *led_gpio)
{
    Tmp102_Init(&tmp, i2c, TMP102_ADDR_GND);
    Bno055_Init(&bno, i2c, BNO055_DEV_ADDR);
    Bmp390_Init(&bmp, i2c, BMP390_DEV_ADDR);

    init_i2c_access(&cli, i2c);
    init_blink(&cli, led_gpio);
    init_read_tmp102(&cli, &tmp);
    init_read_bno055(&cli, &bno);
    init_read_bmp390(&cli, &bmp);

    Command commands[6] = { 
        {"Blink", blink, "Blinks LED."},
        {"Temp", read_tmp102, "Reads temperature."},
        {"Imu", read_bno055, "Reads IMU accel/gyro."},
        {"Baro", read_bmp390, "Reads Barometer Pressure."},
        {"IWrite", write_i2c, "Writes I2c."},
        {"IRead", read_i2c, "Reads I2c."}
    };
    create_cli_task(&cli, usart, commands, 6);
}