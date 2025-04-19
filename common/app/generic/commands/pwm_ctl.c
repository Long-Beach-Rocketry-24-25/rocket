
#include "pwm_ctl.h"

static Pwm* pwm_dev;

static const Command commands[] = {
    {"PwmOutput", pwm_ctl_toggle_out, "Toggles output enable of PWM."},
    {"PwmDuty", pwm_ctl_duty, "Sets duty cycle of PWM as a percentage."}};

void init_pwm_ctl(Cli* cli, Pwm* pwm)
{
    pwm_dev = pwm;
    cli_register_command(cli, commands);
}

void pwm_ctl_toggle_out(int argc, char* argv[])
{
}

void pwm_ctl_duty(int argc, char* argv[])
{
    float duty_cycle;
    sscanf(argv[1], "%f", &duty_cycle);
    pwm_dev->set_duty(pwm_dev, duty_cycle);
}