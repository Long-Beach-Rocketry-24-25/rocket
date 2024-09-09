
#include "FreeRTOS.h"
#include "task.h"

#include "bsp.h"

#include "i2c.h"
#include "i2c_access.h"
#include "usart.h"
#include "usart_cli.h"

#include "tmp102.h"

#include "string_hex.h"

/*-----------------------------------------------------------*/

void blink(int argc, char* argv[]);
void read_temp(int argc, char* argv[]);
void read_imu(int argc, char* argv[]);

Usart usart;
I2c i2c;

Tmp102 tmp;

int main(void)
{

    BSP_Init(&usart, &i2c);

    Tmp102_Init(&tmp, &i2c, TMP102_ADDR_GND);

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
    uint8_t imu_mode = 0x8;
    uint8_t eul_data[6] = {0};
    i2c.set_target(&i2c, 0x28 << 1);
    i2c.write(&i2c, 0x3d, &imu_mode, 1);

    i2c.read(&i2c, 0x1A, eul_data, 6);

    cli_write("Eul x: %d y: %d z: %d",
        (((int16_t) (eul_data[1] << 8)) | ((int16_t) eul_data[0])) >> 4,
        (((int16_t) (eul_data[3] << 8)) | ((int16_t) eul_data[2])) >> 4,
        (((int16_t) (eul_data[5] << 8)) | ((int16_t) eul_data[4])) >> 4
    );
}
