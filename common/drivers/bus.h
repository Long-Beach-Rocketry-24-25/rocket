/**
 * Zephyr driver interface. 
 */

#pragma once

typedef struct
{
    void * init;
    void * data;
    void * app;
    void * extra;
} Device;