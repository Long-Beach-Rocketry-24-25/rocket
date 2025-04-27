
#include "motor_rotater.h"

void MotorRotaterInit(MotorRotater* control, Motor* motor, Encoder* encoder,
                      size_t encoder_counts_per_rotation)
{
    control->state = IDLE;
    control->encoder_counts_per_rotation = encoder_counts_per_rotation;

    control->motor = motor;
    control->encoder = encoder;

    control->curr_distance = 0;
    control->target_distance = 0;
    control->direction = CW;
    control->last_count = 0;
}

void MotorRotaterCommand(MotorRotater* control, bool cmd, float degrees)
{
    control->cmd = cmd;
    control->cmd_degrees = degrees;
}

bool MotorRotaterUpdate(MotorRotater* control)
{
    EXIT_IF(control->motor == NULL || control->encoder == NULL, false);

    bool cmd = control->cmd;
    float degrees = control->cmd_degrees;

    switch (control->state)
    {
        case IDLE:
            if (cmd && degrees != 0)
            {
                control->curr_distance = 0;
                control->target_distance =
                    (size_t)((degrees / 360) * ((degrees < 0) ? -1 : 1) *
                             (float)control->encoder_counts_per_rotation);
                control->direction = (degrees > 0) ? CW : CCW;
                control->last_count =
                    control->encoder->get_counter(control->encoder);

                EXIT_IF(!control->motor->set_direction(control->motor,
                                                       control->direction),
                        false);
                EXIT_IF(!control->motor->set_enabled(control->motor, true),
                        false);
                EXIT_IF(!control->motor->set_power(control->motor, 1), false);

                control->state = ROTATING;
            }
            break;
        case ROTATING:
            if (cmd)
            {
                size_t curr_count =
                    control->encoder->get_counter(control->encoder);

                size_t offset;
                size_t max = control->encoder->get_max_value(control->encoder);
                if (control->direction == CW)
                {
                    offset = (curr_count < control->last_count) ? max : 0;
                    control->curr_distance +=
                        curr_count + offset - control->last_count;
                }
                else
                {
                    offset = (curr_count > control->last_count) ? max : 0;
                    control->curr_distance +=
                        control->last_count + offset - curr_count;
                }

                if (control->curr_distance > control->target_distance)
                {
                    control->state = DONE;
                }

                control->last_count =
                    control->encoder->get_counter(control->encoder);
            }
            else
            {
                control->state = DONE;
            }
            break;
        case DONE:
            EXIT_IF(!control->motor->set_power(control->motor, 0), false);
            EXIT_IF(!control->motor->set_enabled(control->motor, false), false);
            if (!cmd)
            {
                control->state = IDLE;
            }
            break;
    }

    return true;
}

bool MotorRotaterIsActive(MotorRotater* control)
{
    return control->state == ROTATING;
}
