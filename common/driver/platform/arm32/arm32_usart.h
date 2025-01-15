

#pragma once

#include "usart.h"
#include "timeout.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct
{
    Timeout * timer;
} Arm32PrivUsart;

void Arm32UsartInit(Usart *usart, Arm32PrivUsart *st_usart, Timeout* timer);
