

#pragma once

#include "usart.h"
#include "timeout.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct
{
    Timeout * timer;
} NativePrivUsart;

void NativeUsartInit(Usart *usart, NativePrivUsart *st_usart, Timeout* timer);
