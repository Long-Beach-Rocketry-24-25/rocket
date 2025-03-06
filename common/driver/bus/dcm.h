#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "stm32l4xx.h"

typedef struct DCMotor DCMotor;

struct DCMotor
{

    bool (*set_en)(DCMotor *motor, bool enable);
    bool (*set_pos)(DCMotor *motor, bool position);
    void (*set_duty)(DCMotor *motor, float duty);



    void* priv;
};

