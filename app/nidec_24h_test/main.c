
#include "FreeRTOS.h"
#include "task.h"
#include "usart_cli.h"

#include "nidec_bsp.h"

#include "motor_rotater.h"

#include "gpio.h"
#include "pwm.h"

/*-----------------------------------------------------------*/
MotorRotater rotater;
Motor motor;
Encoder encoder;
Cli cli;
Usart usart;

int main(void)
{
    BSP_Init(&usart, &motor, &encoder);

    // motor.set_direction(&motor, CW);
    // motor.set_enabled(&motor, false);

    // motor.set_direction(&motor, CCW);
    // motor.set_enabled(&motor, true);

    // wait = true;
    // while (wait);

    // Fix so 100 not defined here?
    MotorRotaterInit(&rotater, &motor, &encoder, 200);
    while (MotorRotaterUpdate(&rotater, true, -90));
    MotorRotaterUpdate(&rotater, false, 0);

    while (MotorRotaterUpdate(&rotater, true, 90));
    MotorRotaterUpdate(&rotater, false, 0);

    // while (MotorRotaterUpdate(&rotater, true, -90));
    // MotorRotaterUpdate(&rotater, false, 0);

    // while (MotorRotaterUpdate(&rotater, true, 90));
    // MotorRotaterUpdate(&rotater, false, 0);

    // while (MotorRotaterUpdate(&rotater, true, -90));
    // MotorRotaterUpdate(&rotater, false, 0);

    // while (MotorRotaterUpdate(&rotater, true, 90));
    // MotorRotaterUpdate(&rotater, false, 0);

    while (1);
    /* Start the scheduler to start the tasks executing. */
    // vTaskStartScheduler();

    return 0;
}
