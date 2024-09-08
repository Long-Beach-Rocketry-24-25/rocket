
#include "string_hex.h"

uint8_t char_to_hex(char data)
{
    if (data >= '0' && data <= '9')
    {
        return data - '0';
    }
    else if (data >= 'A' && data <= 'F')
    {
        return data - 'A' + 10;
    }
    else if (data >= 'a' && data <= 'f')
    {
        return data - 'a' + 10;
    }
}

bool is_hex(char data)
{
    return (data >= '0' && data <= '9') ||
           (data >= 'A' && data <= 'F') ||
           (data >= 'a' && data <= 'f');
}

uint8_t str_to_8(const char *str)
{
    uint8_t value = 0;
    for (uint8_t i = 0; i < 2; ++i)
    {
        if (str[i] == '\0')
        {
            break;
        }
        else
        {
            if (is_hex(str[i]))
            {
                value <<= 4;
                value |= char_to_hex(str[i]);
            }
        }
    }
    return value;
}

uint16_t str_to_16(const char *str)
{
    uint16_t value = 0;
    for (uint16_t i = 0; i < 4; ++i)
    {
        if (str[i] == '\0')
        {
            break;
        }
        else
        {
            if (is_hex(str[i]))
            {
                value <<= 4;
                value |= char_to_hex(str[i]);
            }
        }
    }
    return value;
}

uint32_t str_to_32(const char *str)
{
    uint32_t value = 0;
    for (uint32_t i = 0; i < 8; ++i)
    {
        if (str[i] == '\0')
        {
            break;
        }
        else
        {
            if (is_hex(str[i]))
            {
                value <<= 4;
                value |= char_to_hex(str[i]);
            }
        }
    }
    return value;
}