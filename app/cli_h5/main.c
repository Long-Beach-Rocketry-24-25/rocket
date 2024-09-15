
#include "FreeRTOS.h"
#include "task.h"

#include "bsp.h"

#include "i2c.h"
#include "i2c_access.h"
#include "usart.h"
#include "usart_cli.h"

#include "tmp102.h"
#include "bno055.h"

#include "string_hex.h"

/*-----------------------------------------------------------*/

void blink(int argc, char* argv[]);
void read_temp(int argc, char* argv[]);
void read_imu(int argc, char* argv[]);

Usart usart;
I2c i2c;

Tmp102 tmp;
Bno055 bno;

int main(void)
{

    BSP_Init(&usart, &i2c);

    Tmp102_Init(&tmp, &i2c, TMP102_ADDR_GND);

    Bno055_Init(&bno, &i2c, BNO055_DEV_ADDR);
    Bno055_Set_Mode(&bno, BNO055_IMU_MODE);

    init_i2c_access(&i2c);

    Command commands[5] = { 
        {"Blink", blink, "Blinks LED."},
        {"Temp", read_temp, "Reads temperature."},
        {"Imu", read_imu, "Reads IMU accel/gyro."},
        {"IWrite", write_i2c, "Writes I2c."},
        {"IRead", read_i2c, "Reads I2c."}
    };
    create_cli_task(&usart, commands, 5);

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
    cli_write("Temp: %dC", (int) x);
}

void read_imu(int argc, char* argv[])
{
    EulerVec e_vec;
    QuaternionVec q_vec;
    bno.get_euler(&bno, &e_vec);
    bno.get_quaternion(&bno, &q_vec);
    uint8_t temp = bno.get_temp_c(&bno);

    cli_write("Eul x: %d y: %d z: %d", e_vec.x, e_vec.y, e_vec.z);
    cli_write("Quat w: %f x: %f y: %f z: %f", q_vec.w, q_vec.x, q_vec.y, q_vec.z);
    cli_write("Temp: %d C", temp);
}
