

#pragma once

#include "usart.h"

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct
{
    int file_handle;
} Arm32PrivUsart;

bool Arm32UsartInit(Usart* usart, Arm32PrivUsart* arm32_usart,
                    const char* path);
void Arm32UsartClose(Usart* usart);
bool Arm32UsartSend(Usart* usart, uint8_t* data, size_t size);
bool Arm32UsartRecv(Usart* usart, uint8_t* data, size_t size);