#include <gtest/gtest.h>
#include <cstdio>
extern "C"
{
#include <string.h>
#include "lbr_net.h"
}

/**
  * @brief Tests for lbr network.
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
/**
 * @brief Test to make sure that the format is giving the expected output.
 */
TEST_F(FormatTests, pack_test)
{
    const uint8_t data[10] = "f";
    const uint8_t expected_checksum =
        (START_TRANSMISSION + ADDRESS + 1 + 'f') % 256;
    const uint8_t expected_buf[] = {START_TRANSMISSION, ADDRESS, 1, 'f',
                                    expected_checksum};
    uint8_t packed[256] = {0};
    send_protocol_init(&bus, ADDRESS);
    bus.pack(&bus, packed, sizeof(packed), ADDRESS, data, 1);
    EXPECT_ARR_EQ(packed, expected_buf, 4);
}

/**
 * @brief Test to make sure NACK changes state to ERROR.
 */
TEST_F(ReadCharTests, idle_to_error_test)
{
    send_protocol_init(&bus, ADDRESS);
    bus.read_byte(&bus, NACK);
    EXPECT_EQ(bus.state, ERROR);
}

/**
 * @brief Test to make sure ACK changes state to ACK.
 */
TEST_F(ReadCharTests, idle_to_ack_test)
{
    send_protocol_init(&bus, ADDRESS);
    bus.read_byte(&bus, ACK);
    EXPECT_EQ(bus.state, ACKNOWLEDGED);
}

/**
 * @brief Test to make sure wrong address goes back to idle.
 */
TEST_F(ReadCharTests, idle_to_wrong_address_test)
{

    const uint8_t data[] = {START_TRANSMISSION, ADDRESS + 1}; /*wrong address*/
    send_protocol_init(&bus, ADDRESS);
    bus.read_byte(&bus, data[0]);
    EXPECT_EQ(bus.state, READ_ADDRESS);
    bus.read_byte(&bus, data[1]);
    EXPECT_EQ(bus.state, IDLE);
}

//stop here for pr
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

//need tests for flush
//need test for get package size