

#pragma once

#include "usart_cli.h"

#include "tmp102.h"
#include "bno055.h"
#include "bmp390.h"

#include "blink.h"
#include "i2c_access.h"
#include "read_tmp102.h"
#include "read_bno055.h"
#include "read_bmp390.h"

void DebugAppCreate(Usart *usart, I2c *i2c, Gpio *led_gpio);