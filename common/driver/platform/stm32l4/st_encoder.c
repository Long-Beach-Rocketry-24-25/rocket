
#include "st_encoder.h"
#include "stm32l476xx.h"

/**
 * Config mask macros. No safety checks, so must bound check
 * before use.
 */
#define CCER_CCxE (1)
#define CCER_CHANNEL_Pos(channel) (((channel) - 1) * 4)
#define CCER_CONFIG_CLEAR(channel) (0xF << CCER_CHANNEL_Pos(channel))
#define CCER_CONFIG_SET(channel) (CCER_CCxE << CCER_CHANNEL_Pos(channel))

bool StEncoderInit(Encoder* encoder, StPrivEncoder* st_enc, size_t channel1,
                   size_t channel2)
{
    if (channel1 < 1 || channel1 > 4 || channel2 < 1 || channel2 > 4)
    {
        return false;
    }

    // Max out reload value.
    st_enc->instance->ARR = UINT32_MAX;

    // Mode select encoder.
    st_enc->instance->SMCR |= 3 << TIM_SMCR_SMS_Pos;

    // Channel input capture enable.
    st_enc->instance->CCER &= ~CCER_CONFIG_CLEAR(channel1);
    st_enc->instance->CCER &= ~CCER_CONFIG_CLEAR(channel2);

    st_enc->instance->CR1 |= TIM_CR1_CEN;

    return true;
}

size_t StEncoderGetCount(Encoder* encoder)
{
    StPrivEncoder* enc = (StPrivEncoder*)encoder->priv;
    (void)enc;
    return 0;
}

size_t StEncoderGetMaxValue(Encoder* encoder)
{
    StPrivEncoder* enc = (StPrivEncoder*)encoder->priv;
    return (size_t)enc->instance->ARR;
}
