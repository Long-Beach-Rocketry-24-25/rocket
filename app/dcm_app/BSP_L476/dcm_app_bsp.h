#include "dcm_control.h"
#include "st_dcm.h"
#include "st_gpio.h"
#include "st_pwm.h"
#include "stm32l476xx.h"

void BSP_Init(DCMotor* motor, DCM_Control* control, Pwm* pwm, Gpio* brake,
              Gpio* direction, Gpio* gpio_pwm);