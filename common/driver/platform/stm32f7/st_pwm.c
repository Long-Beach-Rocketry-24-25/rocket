
#include "st_pwm.h"

bool StPwmInit(Pwm* pwm, StPrivPwm* st_pwm, size_t base_address, size_t channel,
               size_t pclk_freq, size_t timer_size)
{
    st_pwm->instance = (TIM_TypeDef*)base_address;

    if (channel < 1 || channel > 4)
    {
        return false;
    }

    st_pwm->channel = channel;
    st_pwm->pclk_freq = pclk_freq;
    st_pwm->timer_size = timer_size;

    pwm->priv = (void*)st_pwm;
    pwm->enable = StPwmEnable;
    pwm->set_duty = StPwmDuty;
    pwm->set_freq = StPwmSetFreq;

    StPwmEnable(pwm, false);
    StPwmSetFreq(pwm, 1000);
    StPwmDuty(pwm, 0);

    return true;
}

void StPwmEnable(Pwm* pwm, bool enable)
{
    StPrivPwm* dev = (StPrivPwm*)pwm->priv;

    if (enable)
    {
        switch (dev->channel)
        {
            case 1:
                dev->instance->CCMR1 |=
                    TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1PE;
                dev->instance->CCER |= TIM_CCER_CC1E;
                break;
            case 2:
                dev->instance->CCMR1 |=
                    TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2PE;
                dev->instance->CCER |= TIM_CCER_CC2E;
                break;
            case 3:
                dev->instance->CCMR2 |=
                    TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3PE;
                dev->instance->CCER |= TIM_CCER_CC3E;
                break;
            case 4:
                dev->instance->CCMR2 |=
                    TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4PE;
                dev->instance->CCER |= TIM_CCER_CC4E;
                break;
            default:
                break;
        }

        dev->instance->EGR |= TIM_EGR_UG;
        dev->instance->CR1 &= 0;
        dev->instance->CR1 |= TIM_CR1_ARPE | TIM_CR1_CEN;
    }
    else
    {
        dev->instance->CR1 &= ~TIM_CR1_CEN;
    }
}

void StPwmSetFreq(Pwm* pwm, size_t hz)
{
    StPrivPwm* dev = (StPrivPwm*)pwm->priv;

    size_t target_psc = 0;

    if (hz > 0)
    {
        while (target_psc < 1)
        {
            target_psc =
                (size_t)(((double)dev->pclk_freq) / dev->timer_size) / hz;
            dev->timer_size -= dev->timer_size / 20;
        }
    }
    else
    {
        target_psc = 1;
    }

    dev->instance->PSC = target_psc - 1;
    dev->instance->ARR = dev->timer_size - 1;
}

void StPwmDuty(Pwm* pwm, double duty)
{
    StPrivPwm* dev = (StPrivPwm*)pwm->priv;

    size_t cmp = (size_t)((duty / 100) * (dev->instance->ARR));

    dev->instance->CCR3 = cmp;
}