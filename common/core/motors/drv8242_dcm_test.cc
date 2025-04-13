#include <gtest/gtest.h>

extern "C"
{
#include "fake_dcm.h"
}

class Drv8242DcmTest : public testing::Test
{
public:
    DCMotor* motor;
    DCPosControl* dev;
};

TEST_F(Drv8242DcmTest, InitTest)
{
    EXPECT_EQ(dev->enabled, 1);
    // EXPECT_TRUE(motor->set_direction(motor, dev->direction_one));
    // EXPECT_TRUE(motor->set_direction(motor, dev->direction_two));
    // EXPECT_FALSE(motor->set_en(motor, dev->disabled));
    // EXPECT_FALSE(motor->set_direction(motor, dev->direction_one));
}
