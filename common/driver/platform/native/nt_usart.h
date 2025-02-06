

#pragma once

#include "timeout.h"
#include "usart.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct
{
    Timeout* timer;
} NativePrivUsart;

void NativeUsartInit(Usart* usart, NativePrivUsart* st_usart, Timeout* timer);
