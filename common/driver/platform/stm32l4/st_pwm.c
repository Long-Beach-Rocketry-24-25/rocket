
#include <common\driver\bus\st_pwm.h>


bool Pwm_enable(Pwm *pwm, bool enable) {
    StPrivPwm * dev = (StPrivPwm *) pwm->priv;

    if (dev->instance->CR1 & (TIM_CR1_CEN)){
        return false;
    }

    dev->instance->CR1 |= enable;

    return true;
}

void Pwm_set_frequency(Pwm *pwm, size_t freq) {
    StPrivPwm * dev = (StPrivPwm *) = pwm->priv;

    dev->instance->ARR = 10000 - 1; 

    int frq = 84000000/10000                //mc clock divided by ARR
    int div  = frq/freq;                    //dividing by freq we want
    dev->instance->PSC = (frq/div) - 1;     //setting PSC to freq desired
}

void Pwm_set_duty(Pwm *pwm, size_t duty) {
    StPrivPwm *dev = (StPrivPwm *) = pwm->priv;

    dev->instance->CCR1 = duty;
}