
#include <gtest/gtest.h>

extern "C"
{
#include "fake_motor.h"
#include "motor_rotater.h"
}

class MotorTest : public testing::Test
{
public:
    MotorRotater rotater;
    Encoder encoder;
    Motor motor;
};

TEST_F(MotorTest, InitTest)
{
    init_enc(&encoder);
    init_motor(&motor);
    MotorRotaterInit(&rotater, &motor, &encoder, 360);

    float degrees = -20;
    EXPECT_TRUE(MotorRotaterUpdate(&rotater, true, degrees));
    size_t i;
    for (i = 0; i < 12; ++i)
    {
        EXPECT_TRUE(get_mot_en());
        EXPECT_EQ(get_mot_dir(), CCW);
        EXPECT_EQ(get_mot_pct(), 100);
        if (!MotorRotaterUpdate(&rotater, true, 0))
        {
            break;
        }
        decrement(10);
    }
    EXPECT_EQ(i, 3);
}

// TEST_F(RingBufferTest, InsertAndPopTest)
// {
//     ring_buffer_init(&buf, data, BUF_SIZE);
//     for (size_t i = 0; i < BUF_SIZE; ++i)
//     {
//         EXPECT_TRUE(ring_buffer_insert(&buf, i));
//     }

//     EXPECT_FALSE(ring_buffer_insert(&buf, 0));

//     uint8_t data = 0;
//     for (size_t i = 0; i < BUF_SIZE; ++i)
//     {
//         EXPECT_TRUE(ring_buffer_pop(&buf, &data));
//         EXPECT_EQ(i, data);
//     }

//     EXPECT_FALSE(ring_buffer_pop(&buf, &data));
// }
