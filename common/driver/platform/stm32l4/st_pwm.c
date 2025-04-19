
#include "st_pwm.h"

void StPwmInit(Pwm* pwm, StPrivPwm* st_pwm, size_t base_address,
               size_t mc_clock)
{
    st_pwm->instance = (TIM_TypeDef*)base_address;

    pwm->priv = (void*)st_pwm;
    st_pwm->clock = mc_clock;
    pwm->set_duty = StPwmDuty;
    pwm->set_freq = StPwmSetFreq;

    StPwmSetFreq(pwm, 1000);
    StPwmDuty(pwm, 50);  //setting duty cycle to 50%
}

void StPwmConfig(Pwm* pwm)
{
    StPrivPwm* dev = (StPrivPwm*)pwm->priv;

    dev->instance->CCMR1 &= ~TIM_CCMR1_CC1S;
    dev->instance->CCMR1 |= TIM_CCMR1_OC1M_2;
    dev->instance->CCMR1 |= TIM_CCMR1_OC1M_1;
    dev->instance->CCER &= ~TIM_CCER_CC1P;
    dev->instance->CCER |= TIM_CCER_CC1E;

    dev->instance->CCMR1 |= TIM_CCMR1_OC1PE;
    dev->instance->CR1 |= TIM_CR1_ARPE;
    dev->instance->CR1 &= ~TIM_CR1_CMS_Msk;
    dev->instance->CR1 &= ~TIM_CR1_DIR_Msk;
    dev->instance->CR1 &= ~TIM_CR1_CKD_Msk;
    dev->instance->EGR |= TIM_EGR_UG;

    dev->instance->CR1 |= TIM_CR1_CEN;
}

bool StPwmSetFreq(Pwm* pwm, size_t hz)
{
    StPrivPwm* dev = (StPrivPwm*)pwm->priv;

    size_t DesiredPSC;

    if (hz != 0)
    {
        dev->period = 1000 / hz;
        DesiredPSC = ((dev->clock) / 65535) / hz;
    }
    else
    {
        dev->period = 0;
        DesiredPSC = 0;
    }

    dev->instance->PSC = DesiredPSC - 1;
    dev->instance->ARR = 65535 - 1;

    return true;
}

bool StPwmDuty(Pwm* pwm, double duty)
{
    StPrivPwm* dev = (StPrivPwm*)pwm->priv;

    double Duty = (size_t)((duty / 100) * (dev->instance->ARR));

    dev->instance->CCR1 = Duty;

    return true;
}