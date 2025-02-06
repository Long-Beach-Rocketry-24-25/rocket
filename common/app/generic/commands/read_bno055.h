/**
 * 
 */

#pragma once

#include "bno055.h"
#include "send.h"

void init_read_bno055(Send* sender, Bno055* bno_dev);
void read_bno055(int argc, char* argv[]);