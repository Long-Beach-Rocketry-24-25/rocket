#include <gtest/gtest.h>
#include <cstdio>
extern "C"
{
#include "uart_custom_bus.h"
}
#define EXPECT_ARR_EQ(arr1, arr2, size)   \
    do                                    \
    {                                     \
        for (size_t i = 0; i < size; ++i) \
        {                                 \
            EXPECT_EQ(arr1[i], arr2[i]);  \
        }                                 \
    } while (0)

class FormatTest : public testing::Test
{
public:
    Bus bus;
};

TEST_F(FormatTest, format_test)
{
    const char data[10] = "f";
    send_protocol_init(&bus);
    bus.format(&bus, 50, 1, data);
    EXPECT_ARR_EQ(bus.send_buffer, "!2f", 3);
    printf("%d", bus.send_buffer[4]);
    EXPECT_EQ(bus.send_buffer[4], (char)185);
    // need to calculate again without end byte
}
