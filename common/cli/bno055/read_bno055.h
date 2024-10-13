/**
 * 
 */

#pragma once

#include "cli.h"
#include "bno055.h"

void init_read_bno055(Bno055 * bno_dev);
void read_bno055(int argc, char* argv[]);