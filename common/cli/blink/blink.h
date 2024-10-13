/**
 * 
 */

#pragma once

#include "cli.h"
#include "gpio.h"

void init_blink(Gpio * pin);
void blink(int argc, char* argv[]);