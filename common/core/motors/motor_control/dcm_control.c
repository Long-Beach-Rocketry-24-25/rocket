#include "dcm_control.h"

static StPrivPwm st_pwm;
static size_t base_address = TIM2_BASE;
static size_t mc_clock = 84000000;

static StGpioParams st_nSleep_pwm = {{0}, GPIOA_BASE, 1, {GPOUT, 0, 0, 1, 0}};
static StGpioParams st_drvoff_pin = {{0}, GPIOA_BASE, 2, {GPOUT, 0, 0, 2, 0}};
static StGpioParams st_en_in1_pin = {{0},
                                     GPIOA_BASE,
                                     0,
                                     {ALT_FUNC, 0, 0, 0, 0x1}};
static StGpioParams st_ph_in2_pin = {{0}, GPIOA_BASE, 3, {GPOUT, 0, 0, 0, 0}};

static DCPosControl* control;

void DCM_Control_Init(Pwm* pwm_timer, DCMotor* motor, Gpio* nSleep_pwm,
                      Gpio* drvoff_pin, Gpio* en_in1_pin, Gpio* ph_in2_pin)
{

    // LED GPIO
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

    StGpioInit(nSleep_pwm, &st_nSleep_pwm);
    StGpioInit(drvoff_pin, &st_drvoff_pin);
    StGpioInit(en_in1_pin, &st_en_in1_pin);
    StGpioInit(ph_in2_pin, &st_ph_in2_pin);
    StGpioConfig(nSleep_pwm);
    StGpioConfig(drvoff_pin);
    StGpioConfig(en_in1_pin);
    StGpioConfig(ph_in2_pin);

    StPwmInit(pwm_timer, &st_pwm, base_address, mc_clock);
    Drv8242Init(motor, control, nSleep_pwm, drvoff_pin, en_in1_pin, ph_in2_pin,
                pwm_timer);
}

// void config(DCM_Control* dc_control, size_t angle)
// {
//     size_t ticks_per_angle = dc_control->pusle_per_rev / 90;
//     size_t encoder_ticks = angle * ticks_per_angle;
// }

// void update()
// {
//     switch (control->state)
//     {
//         case IDLE:
//             if (control->cmd)
//             {
//                 ...
//             }
//     }
// }