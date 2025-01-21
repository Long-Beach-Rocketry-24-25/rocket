/**
 * 
 */

#pragma once

#include "send.h"
#include "gpio.h"

void init_blink(Send *sender, Gpio * pin);
void blink(int argc, char* argv[]);