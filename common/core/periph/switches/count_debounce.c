#include "count_debounce.h"
//#include "core_cm4.h" // SysTick
#include <stdint.h>

//initialization
void CountDebounce_Init(CountDebounce* db, IoInput* input, uint8_t threshold)
{
    db->state = DEBOUNCE_OFF; // its gonna start at released
    db->counter = 0;
    db->threshold = threshold;

    input->get_state = CountDebounce_GetStateFromIoInput;
    input->priv = (void*) db;
}

// updating the logic lowk gonna call this a lot IMPORTANT
void CountDebounce_Update(CountDebounce* db, IoInput* input)
{
    bool raw_state = input->get_state(input); // reads the input here

    // if the "raw" state is different from stable its gonna count
    if ((raw_state && db->state == DEBOUNCE_OFF) || 
        (!raw_state && db->state == DEBOUNCE_ON)) 
    {
        db->counter++;

        // when counter exceeds the threshold then we have to update the state within
        if (db->counter >= db->threshold) 
        {
            db->state = raw_state ? DEBOUNCE_ON : DEBOUNCE_OFF;
            db->counter = 0;  //resets counter after it switches it's state
        }
    } 
    else 
    {
        db->counter = 0;  // counter resettt 
    }
}

//this does not update debounce logic
//returns only stable state
//have to call CountDebounce_Update() regularly to stay current
bool CountDebounce_GetStateFromIoInput(IoInput* input)
{
    CountDebounce* db = (CountDebounce*) input->priv;
    return db->state == DEBOUNCE_ON;
}

DebounceState CountDebounce_GetState(CountDebounce* db)
{
    return db->state;
}

//true is button is pressed and stable
bool CountDebounce_IsPressed(CountDebounce* db)
{
    return db->state == DEBOUNCE_ON;
}

//true if button is released at stable
bool CountDebounce_IsReleased(CountDebounce* db)
{
    return db->state == DEBOUNCE_OFF;
}