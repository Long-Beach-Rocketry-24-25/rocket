/**
 * 
 */

#pragma once

#include "altitude.h"
#include "bmp390.h"
#include "send.h"

void init_read_bmp390(Send* sender, Bmp390* bmp_dev);
void read_bmp390(int argc, char* argv[]);