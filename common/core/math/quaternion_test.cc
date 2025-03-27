
#include <gtest/gtest.h>

extern "C"
{
#include "quaternion.h"
}

#define EXPECT_QUAT_EQ(q1, q2) EXPECT_TRUE(quat_compare(q1, q2))

class QuaternionTest : public testing::Test
{
};

TEST_F(QuaternionTest, QuatTest)
{
    QuaternionVec a = {1, 1, 1, 1};
    QuaternionVec res = {1, 1, 1, 1};
    EXPECT_TRUE(quat_compare(&a, &res));

    {
        QuaternionVec exp = {1, -1, -1, -1};
        EXPECT_QUAT_EQ(quat_conjugate(&a, &res), &exp);
    }

    {
        QuaternionVec ac = {0, 0, 0, 9.8};
        QuaternionVec o_c;
        QuaternionVec o = {.707, -.707, 0, 0};
        QuaternionVec intrm;
        // QuaternionVec exp = {0, 0, 0, 9.8};

        // Extracting vertical acceleration
        quat_conjugate(&o, &o_c);
        quat_multiply(&o, &ac, &intrm);
        quat_multiply(&intrm, &o_c, &res);
        printf("q: %f %f %f %f\n", res.w, res.x, res.y, res.z);
    }
}
