#include <gtest/gtest.h>

extern "C"
{
#include "drv8242_enc.h"
}

class Drv8242EncTest : public testing::Test
{
public:
    QEnc* qenc;
    MotorRotoationCtrler* controller;
};

TEST_F(Drv8242EncTest, InitTest)
{
    EXPECT_TRUE(controller->command_rotate(controller, 0), 0);
    EXPECT_TRUE(controller->command_rotate(controller, 100), 1);
    EXPECT_TRUE(controller->update(controller), 1);
    EXPECT_TRUE(controller->update(controller), 0);
    EXPECT_TRUE(controller->command_rotate(controller, 100), 0);
}
