/**
 * 
 * 
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

//typedef struct Usart Usart;

//struct Usart
//{
//    bool (*send)(Usart* usart, const uint8_t* data, size_t size);
//     bool (*recv)(Usart* usart, uint8_t* data, size_t size);
//
//    void* priv;
//};

//abstraction layer
// interface

typedef struct IoInput IoInput;

struct IoInput
{
    // return true if io is 1 or or false if 0
    bool (*get_state)(IoInput* in);
    //got rid of get_state_raw
    void* priv;
};
