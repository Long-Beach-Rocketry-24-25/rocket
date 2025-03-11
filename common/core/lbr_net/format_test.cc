#include <gtest/gtest.h>
#include <cstdio>
extern "C"
{
#include <string.h>
#include "uart_custom_bus.h"
}

/**
  * @brief tests for lbr network
  */

#define ADDRESS 69
#define EXPECT_ARR_EQ(arr1, arr2, size)   \
    do                                    \
    {                                     \
        for (size_t i = 0; i < size; ++i) \
        {                                 \
            EXPECT_EQ(arr1[i], arr2[i]);  \
        }                                 \
    } while (0)

class FormatTests : public testing::Test
{
public:
    Bus bus;
};

class ReadCharTests : public testing::Test
{
public:
    Bus bus;
};

TEST_F(FormatTests, format_test)
{
    /**
  * @brief test to make sure that the format is giving the expected output
  * @param format_test test object
  */
    // data buffer that we want to send with the char f
    const uint8_t data[10] = "f";
    // expected format excluding the checksum.
    const uint8_t expected_buf[] = {START_TRANSMISSION, ADDRESS, 1, 'f'};
    uint8_t formatted[256] = {0};
    send_protocol_init(&bus, ADDRESS);
    uint64_t sum = 0;
    bus.format(&bus, formatted, sizeof(formatted), ADDRESS, data, 1);
    EXPECT_ARR_EQ(formatted, expected_buf, 3);

    EXPECT_EQ(formatted[4], uint8_t(186));
}

TEST_F(ReadCharTests, idle_to_error_test)
{
    const char data[10] = "-";
    send_protocol_init(&bus, ADDRESS);
    bus.read_byte(&bus, data[0]);
    EXPECT_EQ(bus.state, ERROR);
}
TEST_F(ReadCharTests, idle_to_ack_test)
{
    const char data[10] = "+";
    send_protocol_init(&bus, ADDRESS);
    bus.read_byte(&bus, data[0]);
    EXPECT_EQ(bus.state, ACKNOWLEDGED);
}
TEST_F(ReadCharTests, idle_to_wrong_address_test)
{
    const char data[] = "!F";
    send_protocol_init(&bus, ADDRESS);
    bus.read_byte(&bus, data[0]);
    EXPECT_EQ(bus.state, READ_ADDRESS);
    bus.read_byte(&bus, data[1]);
    EXPECT_EQ(bus.state, IDLE);
}
TEST_F(ReadCharTests, idle_to_success)
{
    const char data[] = {'!', 'E', 2, 'P', 'f', 30};
    send_protocol_init(&bus, ADDRESS);
    for (int i = 0; i < sizeof(data); i++)
    {
        bus.read_byte(&bus, data[i]);
    }
    EXPECT_EQ(bus.state, FINISHED);
    EXPECT_ARR_EQ(bus.receive_buffer, data, 6);
}
TEST_F(ReadCharTests, idle_wrong_checksum)
{
    const char data[] = {'!', 'E', 2, 'P', 'f', 32};
    send_protocol_init(&bus, ADDRESS);
    for (int i = 0; i < sizeof(data); i++)
    {
        bus.read_byte(&bus, data[i]);
    }

    EXPECT_EQ(bus.state, ERROR);
}