/**
 * 
 */

#pragma once

#include "cli.h"
#include "bmp390.h"
#include "altitude.h"

void init_read_bmp390(Bmp390 * bmp_dev);
void read_bmp390(int argc, char* argv[]);