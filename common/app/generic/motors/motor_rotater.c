
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

bool MotorRotaterUpdate(MotorRotater* control, bool cmd, float degrees)
{
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

                control->motor->set_direction(control->motor,
                                              control->direction);
                control->motor->set_enabled(control->motor, true);
                control->motor->set_power(control->motor, 100);

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
            control->motor->set_enabled(control->motor, false);
            if (!cmd)
            {
                control->state = IDLE;
            }
            break;
    }

    return control->state == ROTATING;
}