
#pragma once

#include "encoder.h"
#include "motor.h"

static uint16_t counter;

static size_t get_count(Encoder* enc)
{
    return (size_t)counter;
}

static size_t get_max(Encoder* enc)
{
    return UINT16_MAX;
}

void increment(size_t counts)
{
    counter += counts;
}

void decrement(size_t counts)
{
    counter -= counts;
}

void init_enc(Encoder* enc)
{
    enc->get_counter = get_count;
    enc->get_max_value = get_max;
}

static bool enabled = false;
static MotorDirection dir = CW;
static float pctg = 0;

static bool set_en(Motor* motor, bool enable)
{
    enabled = enable;
    return true;
}

static bool set_dir(Motor* motor, MotorDirection direction)
{
    dir = direction;
    return true;
}

static bool set_pwr(Motor* motor, float percentage)
{
    pctg = percentage;
    return true;
}

bool get_mot_en()
{
    return enabled;
}

MotorDirection get_mot_dir()
{
    return dir;
}

float get_mot_pct()
{
    return pctg;
}

void init_motor(Motor* motor)
{
    motor->set_enabled = set_en;
    motor->set_direction = set_dir;
    motor->set_power = set_pwr;
    motor->priv = NULL;
}
