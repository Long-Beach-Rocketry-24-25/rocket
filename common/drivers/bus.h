/**
<<<<<<< HEAD
 * Zephyr driver interface. 
=======
 * 
 * 
>>>>>>> c047760 (nf-f7: experimental)
 */

#pragma once

<<<<<<< HEAD
typedef struct
{
    void * init;
    void * data;
    void * app;
    void * extra;
} Device;
=======
#include <stdbool.h>
#include <stdint.h>

typedef void (*write) (void * dev, uint8_t *data, size_t size);
typedef bool (*read) (void * dev, uint8_t *data, size_t size);

typedef struct
{
    write write;
    read read;
    void * dev;
    void * extra;
} Bus;
>>>>>>> c047760 (nf-f7: experimental)
