/**
 * 
 * 
 */


#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    bool (*write_str)(const char *data, size_t size);
    bool (*read_str)(char *data, size_t size);
} StrComm;
