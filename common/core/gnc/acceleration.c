
#include "acceleration.h"

ThreeAxisVec* accel_rotate(ThreeAxisVec* accel, QuaternionVec* orientation,
                           ThreeAxisVec* result)
{
    QuaternionVec accel_v = {0, accel->x, accel->y, accel->z};
    QuaternionVec orient_conj;
    QuaternionVec intermediate;
    QuaternionVec result_v;

    quat_conjugate(orientation, &orient_conj);
    quat_multiply(orientation, &accel_v, &intermediate);
    quat_multiply(&intermediate, &orient_conj, &result_v);

    result->x = result_v.x;
    result->y = result_v.y;
    result->z = result_v.z;

    return result;
}

double accel_to_velocity(double last_velocity, double accel, double time_s)
{
    return last_velocity + (accel * time_s);
}
