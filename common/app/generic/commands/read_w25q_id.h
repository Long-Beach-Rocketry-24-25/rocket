/**
 * 
 */

#pragma once

#include "send.h"
#include "spi.h"

void init_read_w25q_id(Send *sender, Spi *spi);
void read_w25q_id(int argc, char* argv[]);
