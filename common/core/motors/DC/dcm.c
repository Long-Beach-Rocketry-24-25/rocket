#include "dcm.h"
#include "pwm_app_bsp.h"

DCPosControl config;

bool set_en(DCMotor* motor, bool enable, Gpio* gpio, Pwm* pwm)
{

    config.motor_pin_1 = {};
    config.motor_pin_2 = {};  //fix later
    config.motor_pin_3 = {};

    BSP_Init(pwm, gpio, &config.motor_pin_1);
    BSP_Init(pwm, gpio, &config.motor_pin_2);
    BSP_Init(pwm, gpio, &config.motor_pin_2);

    StPwmEnable(pwm, true);

    //will add more
}
