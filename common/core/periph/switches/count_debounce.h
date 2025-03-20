#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "io_input.h"

//will define actual debouncing logic to use in c file

//two debounce states, on and off

typedef enum
{
    DEBOUNCE_OFF = 0,  // button when released
    DEBOUNCE_ON = 1    // button when pressed
} DebounceState;

typedef struct
{
    DebounceState state;  //stable state
    uint8_t counter;      //count opposite readings
    //if n opposite values in a row - > change the state
    //else stay in current state and reset counter
    uint8_t threshold;  //switch states
} CountDebounce;

//initiate debounce
//db is pointer to debouncer
// threshold is # of opposite reads before changing state
void CountDebounce_Init(CountDebounce* db, IoInput* input, uint8_t threshold);

//to update
void CountDebounce_Update(CountDebounce* db, IoInput* input);

//on or off? hmmm this will tell us
DebounceState CountDebounce_GetState(CountDebounce* db);

//true when pressed
bool CountDebounce_IsPressed(CountDebounce* db);

//true when released
bool CountDebounce_IsReleased(CountDebounce* db);

//state function  to use when inside an io input
bool CountDebounce_GetStateFromIoInput(IoInput* input);