#include <gtest/gtest.h>

extern "C"
{
#include "fake_dcm.h"
#include "fake_dcm_control.h"
#include "fake_enc.h"
}

class DcmControlTest : public testing::Test
{
public:
    DCM_Control dev;
    DCPosControl motor_ctrl;
    MotorRotoationCtrler qenc_ctrl;
    DCMotor motor;
    QEnc qenc;
};

TEST_F(DcmControlTest, InitTest)
{
    FakeInit(&motor, &motor_ctrl);
    QEnc_Init(&qenc, &qenc_ctrl, &motor);
    DCM_Control_Init(&dev, &motor_ctrl, &qenc_ctrl);
    FakeSetEnable(&motor, true);
    config(&dev);
    EXPECT_TRUE(qenc_ctrl.command_rotate(&qenc_ctrl, 10));
    EXPECT_EQ(qenc_ctrl.state, idle);
    motor_update(&dev);
    EXPECT_EQ(qenc_ctrl.state, rotating);
    EXPECT_TRUE(dev.cmd_enable);
}
