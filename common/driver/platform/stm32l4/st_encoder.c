
#include "st_encoder.h"

#define SMCR_BOTH_EDGES (3 << TIM_SMCR_SMS_Pos)

/**
 * Config mask macros. No safety checks, so must bound check
 * before use.
 */
#define CCER_CCxE (1)
#define CCER_CHANNEL_Pos(channel) (((channel) - 1) * 4)

// CCER config for non-inverted input, CCxE = 0, CC1P/CC1NP = 0.
#define CCER_CONFIG_CLEAR(channel) (0xF << CCER_CHANNEL_Pos(channel))

bool StEncoderInit(Encoder* encoder, StPrivEncoder* st_enc, uint32_t base_addr,
                   size_t channel_pair, StEncoderCmpChannel channel_polarity)
{
    if (channel_pair < 0 || channel_pair > ST_ENCODER_CHANNEL_PAIR_MAX)
    {
        return false;
    }

    st_enc->instance = (TIM_TypeDef*)base_addr;

    // Max out reload value to all F's.
    st_enc->instance->ARR = UINT32_MAX;

    // Mode select encoder.
    st_enc->instance->SMCR |= SMCR_BOTH_EDGES;

    // Channel input capture enable.
    st_enc->instance->CCER &= ~CCER_CONFIG_CLEAR(channel_pair + 1);
    st_enc->instance->CCER &= ~CCER_CONFIG_CLEAR(channel_pair + 2);

    switch (channel_pair)
    {
        case 1:
            st_enc->instance->CCMR1 |=
                (channel_polarity << TIM_CCMR1_CC1S_Pos) |
                (channel_polarity << TIM_CCMR1_CC2S_Pos);
            break;
        case 2:
            st_enc->instance->CCMR2 |=
                (channel_polarity << TIM_CCMR2_CC3S_Pos) |
                (channel_polarity << TIM_CCMR2_CC4S_Pos);
            break;
        default:
            return false;
            break;
    }

    st_enc->instance->CR1 |= TIM_CR1_CEN;

    encoder->priv = (void*)st_enc;
    encoder->get_counter = StEncoderGetCount;
    encoder->get_max_value = StEncoderGetMaxValue;
    encoder->counts_per_cycle = StEncoderCountsPerCycle;

    return true;
}

size_t StEncoderGetCount(Encoder* encoder)
{
    StPrivEncoder* enc = (StPrivEncoder*)encoder->priv;
    return (size_t)enc->instance->CNT;
}

size_t StEncoderGetMaxValue(Encoder* encoder)
{
    StPrivEncoder* enc = (StPrivEncoder*)encoder->priv;
    return (size_t)enc->instance->ARR;
}

size_t StEncoderCountsPerCycle(Encoder* encoder)
{
    /** 
     * Configured for rising, falling edge for each signal,
     * and to count both A and B signals - 2 * 2 = 4.
     */
    return 4;
}
