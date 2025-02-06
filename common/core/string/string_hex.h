/**
 * 
 * 
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

uint8_t char_to_hex(char data);

bool is_hex(char data);

uint8_t str_to_8(const char* str);

uint16_t str_to_16(const char* str);

uint32_t str_to_32(const char* str);