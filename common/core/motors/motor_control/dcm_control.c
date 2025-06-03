#include "dcm_control.h"
#include <math.h>
#include "stm32l4xx_hal.h"
#

void DCM_Control_Init(DCM_Control* control, DCMotor* motor,
                      size_t pulse_per_rev)
{

    control->motor = motor;
    control->pusle_per_rev = pulse_per_rev;

    control->state = IDLE;
    control->target_count = 0;
    control->cmd_degrees = 0;
    control->prev_pos = 0;
    control->curr_pos = 0;
    control->dir = 1;
    control->diff = 0;
    control->cmd = false;
}

void DcmControlCommand(DCM_Control* control, bool command, float degrees)
{
    control->cmd = command;
    control->cmd_degrees = degrees;
    StDcmSetEnable(control->motor, command);
    if (command)
    {
        control->state = IDLE;
    }
    // else
    // {
    //     control->curr_pos = 0;
    //     control->diff = 0;
    //     control->prev_pos = 0;
    //     control->state = CCW;
    // }
}

bool DcmControlUpdate(DCM_Control* control)
{
    control->curr_pos += 5;
    if (control->cmd)
    {
        switch (control->state)
        {
            case IDLE:
                control->target_count =
                    ((control->cmd_degrees / 360) *
                     ((control->cmd_degrees < 0) ? -1 : 1)) *
                    (control->pusle_per_rev);

                control->curr_pos = 0;
                control->motor->set_direction(control->motor, control->dir);
                control->motor->set_duty(control->motor, 1);  //10% duty cycle
                control->state = RUNNING;
            case RUNNING:
                control->diff += (control->curr_pos - control->prev_pos);
                if (fabs(control->diff) < fabs(control->target_count))
                {
                }
                else
                {
                    control->cmd = false;
                    DcmControlCommand(control, control->cmd, 0);
                    control->motor->set_duty(control->motor, 0);
                    // HAL_Delay(500);
                    // control->cmd = true;
                    // DcmControlCommand(control, control->cmd, 100);
                    // control->motor->set_direction(control->motor, 0);
                    control->state = DONE;
                    // control->state = CCW;
                }
            // case CCW:
            //     control->diff += (control->curr_pos - control->prev_pos);
            //     if (fabs(control->diff) < fabs(control->target_count))
            //     {
            //     }
            //     else
            //     {
            //         control->cmd = false;
            //         DcmControlCommand(control, control->cmd, 0);
            //         control->motor->set_duty(control->motor, 0);
            //         control->state = DONE;
            //     }
            case DONE:
                if (!control->cmd)
                {
                    control->curr_pos = 0;
                    control->diff = 0;
                    control->prev_pos = 0;
                    control->state = IDLE;
                }
        }

        control->prev_pos = control->curr_pos;
    }
    else
    {
        control->state = IDLE;
        return false;
    }
    return true;
}