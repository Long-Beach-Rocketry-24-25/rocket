
#include "FreeRTOS.h"
#include "task.h"
#include "usart_cli.h"

#include "nidec_bsp.h"

#include "main_loop.h"
#include "motor_rotater.h"

#include "gpio.h"
#include "pwm.h"

/*-----------------------------------------------------------*/
MotorRotater rotater;
Motor motor;
Encoder encoder;
Cli cli;
Usart usart;

static void motor_loop(void)
{
    if (MotorRotaterIsActive(&rotater))
    {
        cli.comm->fwrite(cli.comm, "Enc: %zu", encoder.get_counter(&encoder));
    }
    MotorRotaterUpdate(&rotater);
}

void command_motor(int argc, char* argv[])
{
    float degrees;
    sscanf(argv[1], "%f", &degrees);
    cli.comm->fwrite(cli.comm, "Commanding motor to rotate %f degrees.",
                     degrees);
    MotorRotaterCommand(&rotater, true, degrees);
}

void motor_ack(int argc, char* argv[])
{
    MotorRotaterCommand(&rotater, false, 0);
}

#define NUM_MOTOR_CMDS 2

Command motor_cmds[NUM_MOTOR_CMDS] = {
    {"MotorRotate", command_motor,
     "Command the motor to rotate x degrees, i.e. "
     "MotorRotate 90 for 90 degrees CW."},
    {"MotorAck", motor_ack,
     "Send command false to acknowledge end of rotation procedure."}};

int main(void)
{
    BSP_Init(&usart, &motor, &encoder);

    create_cli_task(&cli, &usart, motor_cmds, NUM_MOTOR_CMDS);

    create_main_loop(motor_loop, 1000);

    // Fix so encoder counts not cfged here?
    MotorRotaterInit(&rotater, &motor, &encoder, 400);

    // while (1)
    // {
    //     volatile float deg = 90;
    //     volatile bool wait = true;
    //     while (wait);

    //     MotorRotaterCommand(&rotater, true, deg);
    //     MotorRotaterUpdate(&rotater);
    //     while (MotorRotaterIsActive(&rotater))
    //     {
    //         cli.comm->fwrite(cli.comm, "Enc: %zu",
    //                          encoder.get_counter(&encoder));
    //         MotorRotaterUpdate(&rotater);
    //     }
    //     MotorRotaterUpdate(&rotater);
    //     MotorRotaterCommand(&rotater, false, 0);
    //     MotorRotaterUpdate(&rotater);
    // }

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    return 0;
}
