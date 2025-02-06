/**
 * 
 */

#pragma once

#include "send.h"
#include "tmp102.h"

void init_read_tmp102(Send* sender, Tmp102* tmp_dev);
void read_tmp102(int argc, char* argv[]);