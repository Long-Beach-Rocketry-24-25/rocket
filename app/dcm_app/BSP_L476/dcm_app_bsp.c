#include "dcm_app_bsp.h"
#include "stm32l4xx_hal.h"

static DCPosControl st_motor;
//static DCM_Control control;
static StPrivPwm st_pwm;
static StGpioParams direction_param = {{0},
                                       GPIOA_BASE,
                                       0,
                                       {GPOUT, 0, 0, 0, 0x0}};  //GPIOA0 ->
static StGpioParams brake_param = {{0}, GPIOA_BASE, 1, {GPOUT, 0, 0, 0, 0x0}};
static StGpioParams pwm_param = {{0}, GPIOA_BASE, 6, {ALT_FUNC, 0, 0, 0, 0x2}};

void BSP_Init(DCMotor* motor, DCM_Control* control, Pwm* pwm, Gpio* brake,
              Gpio* direction, Gpio* gpio_pwm)
{
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    // RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN;

    StGpioInit(brake, &brake_param);
    StGpioConfig(brake);

    StGpioInit(direction, &direction_param);
    StGpioConfig(direction);

    StGpioInit(gpio_pwm, &pwm_param);
    StGpioConfig(gpio_pwm);

    StPwmInit(pwm, &st_pwm, TIM3_BASE, 84000000);

    StDcmInit(motor, &st_motor, brake, direction, pwm);
    DCM_Control_Init(control, motor, 100);
}