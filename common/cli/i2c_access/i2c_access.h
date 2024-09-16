/**
 * 
 * 
 */

#pragma once

#include "cli.h"
#include "string_hex.h"

#include "i2c.h"

#define MAX_I2CW_SIZE 16
#define MAX_I2CR_SIZE 6

void init_i2c_access(I2c *i2c_dev);
void write_i2c(int argc, char* argv[]);
void read_i2c(int argc, char* argv[]);