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

#define ADDRESS 70
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

/**
 * @brief Test if a complete message turns state to finished, and check if flushing clears and stores.
 */
TEST_F(ReadCharTests, idle_to_success_then_flush)
{
    uint8_t sum = ('!' + ADDRESS + 2 + 'P' + 'f') % 256;
    const uint8_t data[] = {'!', ADDRESS, 2, 'P', 'f', sum};
    const uint8_t msg[] = {'P', 'f'};
    send_protocol_init(&bus, ADDRESS);
    printf("%u", bus.address);
    for (int i = 0; i < sizeof(data); i++)
    {
        bus.read_byte(&bus, data[i]);
    }
    EXPECT_EQ(bus.state, FINISHED);
    EXPECT_ARR_EQ(bus.receive_buffer, data, 6);
    uint8_t flushed[6] = {0};
    uint8_t empty[255] = {0};
    bus.receive_flush(&bus, flushed);
    EXPECT_ARR_EQ(flushed, msg, bus.package_size);
    EXPECT_ARR_EQ(bus.receive_buffer, empty, 255);
}

/**
 * @brief Test that a wrong checksum will result in an error.
 */
TEST_F(ReadCharTests, idle_wrong_checksum)
{
    const char data[] = {'!', ADDRESS, 2, 'P', 'f', 32};
    send_protocol_init(&bus, ADDRESS);
    for (int i = 0; i < sizeof(data); i++)
    {
        bus.read_byte(&bus, data[i]);
    }

    EXPECT_EQ(bus.state, ERROR);
}

/**
 * @brief Test encode then decode.
 */
TEST_F(FormatTests, encode_decode_test)
{
    const uint8_t data[5] = {'c', 'a', 'f', 'e', 's'};
    const uint8_t expected_checksum =
        (START_TRANSMISSION + ADDRESS + 5 + 'c' + 'a' + 'f' + 'e' + 's') % 256;
    const uint8_t expected_buf[] = {
        START_TRANSMISSION, ADDRESS, 5, 'c', 'a', 'f', 'e', 's',
        expected_checksum};
    uint8_t packed[256] = {0};
    send_protocol_init(&bus, ADDRESS);

    bus.pack(&bus, packed, sizeof(packed), ADDRESS, data, 5);

    for (int i = 0; i < sizeof(expected_buf); i++)
    {
        bus.read_byte(&bus, packed[i]);
    }
    EXPECT_EQ(bus.state, FINISHED);
    EXPECT_ARR_EQ(bus.receive_buffer, packed, sizeof(expected_buf));

    uint8_t flushed[9] = {0};
    uint8_t empty[255] = {0};
    bus.receive_flush(&bus, flushed);

    EXPECT_ARR_EQ(bus.receive_buffer, empty, 255);
    EXPECT_ARR_EQ(flushed, data, 5);
}