/**
 * 
 */

#pragma once

#include "gpio.h"
#include "send.h"

void init_blink(Send* sender, Gpio* pin);
void blink(int argc, char* argv[]);