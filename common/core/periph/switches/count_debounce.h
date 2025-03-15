#pragma once

#include "io_input.h"
#include <stdint.h>
#include <stdbool.h>

//will define actual debouncing logic to use in c file

//two debounce states, on and off

typedef enum {
    DEBOUNCE_OFF = 0,  // button when released
    DEBOUNCE_ON = 1       // button when pressed
} DebounceState;

typedef struct {
    IoInput* input;     //this will be the input source so GPIO
    DebounceState state;   //current state 
    uint8_t counter;      //count for opposite readings 
    //if n opposite values in a row - > change the state
    //else stay in current state and reset counter
    uint8_t threshold;   //threshold b4 we switch states
} CountDebounce;

//initiate debounce 
//db is pointer to debouncer
// threshold is # of opposite reads before changing state
void CountDebounce_Init(CountDebounce* db, IoInput* input, uint8_t threshold);

//to update 
void CountDebounce_Update(CountDebounce* db);

//on or off? hmmm this will tell us
DebounceState CountDebounce_GetState(CountDebounce* db);

//true when pressed
bool CountDebounce_IsPressed(CountDebounce* db);

//true when released
bool CountDebounce_IsReleased(CountDebounce* db);