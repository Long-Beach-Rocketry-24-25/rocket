
#include "FreeRTOS.h"
#include "task.h"

#include "bsp.h"

#include "i2c.h"
#include "i2c_access.h"
#include "usart.h"
#include "usart_cli.h"

#include "tmp102.h"
#include "bno055.h"
#include "bmp390.h"

#include "string_hex.h"

#include <math.h>

/*-----------------------------------------------------------*/

void blink(int argc, char* argv[]);
void read_temp(int argc, char* argv[]);
void read_imu(int argc, char* argv[]);
void read_baro(int argc, char* argv[]);

Usart usart;
I2c i2c;

Tmp102 tmp;
Bno055 bno;
Bmp390 bmp;

int main(void)
{

    BSP_Init(&usart, &i2c);

    Tmp102_Init(&tmp, &i2c, TMP102_ADDR_GND);

    Bno055_Init(&bno, &i2c, BNO055_DEV_ADDR);

    Bmp390_Init(&bmp, &i2c, BMP390_DEV_ADDR);

    init_i2c_access(&i2c);

    Command commands[6] = { 
        {"Blink", blink, "Blinks LED."},
        {"Temp", read_temp, "Reads temperature."},
        {"Imu", read_imu, "Reads IMU accel/gyro."},
        {"Baro", read_baro, "Reads Barometer Pressure."},
        {"IWrite", write_i2c, "Writes I2c."},
        {"IRead", read_i2c, "Reads I2c."}
    };
    create_cli_task(&usart, commands, 6);

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    for( ; ; )
    {
    }

    return 0;
}

void blink(int argc, char* argv[])
{
    GPIOB->ODR ^= GPIO_ODR_OD0;
    cli_write("Blink - %d", GPIOB->ODR & GPIO_ODR_OD0);
}

void read_temp(int argc, char* argv[])
{
    float x = tmp.get_temp_c(&tmp);
    cli_write("Temp: %fC", x);
}

void read_imu(int argc, char* argv[])
{
    Bno055_Set_Mode(&bno, BNO055_IMU_MODE);
    EulerVec e_vec;
    QuaternionVec q_vec;
    bno.get_euler(&bno, &e_vec);
    bno.get_quaternion(&bno, &q_vec);
    uint8_t temp = bno.get_temp_c(&bno);

    cli_write("Eul x: %d y: %d z: %d", e_vec.x, e_vec.y, e_vec.z);
    cli_write("Quat w: %f x: %f y: %f z: %f",
        q_vec.w,
        q_vec.x,
        q_vec.y,
        q_vec.z
    );
    cli_write("Temp: %d C", temp);
}

void read_baro(int argc, char* argv[])
{

    // uint8_t conf = 0x33; // 31
    // uint8_t data[6] = {0};
    // i2c.write(&i2c, 0x1B, &conf, 1);
    // i2c.read(&i2c, 0x4, data, 3);
    // uint32_t res = ((data[0] << 16) | (data[1] << 8) | data[2]);
    // float atmos = res / 2.64;

    Bmp390_Config(&bmp);

    float press_pa = bmp.get_pressure_pa(&bmp);
    float temp_c = bmp.get_temp_c(&bmp);

    cli_write("Pressure: %f Pa, Temp: %f C", press_pa, temp_c);
    // cli_write("Pressure: %f Pa", 44331.0 * (1.0 - pow((atmos / 101325.0), 0.1903)));
}
