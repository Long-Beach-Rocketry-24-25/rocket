
#include "read_bno055.h"

static Send* send;
static Bno055* bno;

void init_read_bno055(Send* sender, Bno055* bno_dev)
{
    bno = bno_dev;
    send = sender;
}

void read_bno055(int argc, char* argv[])
{
    Bno055_Set_Mode(bno, BNO055_IMU_MODE);
    EulerVec e_vec;
    QuaternionVec q_vec;
    ThreeAxisVec accel;
    bno->get_accel(bno, &accel);
    bno->get_euler(bno, &e_vec);
    bno->get_quaternion(bno, &q_vec);
    uint8_t temp = bno->get_temp_c(bno);

    send->fwrite(send, "Accel x: %f y: %f z: %f", accel.x, accel.y, accel.z);
    send->fwrite(send, "Eul x: %d y: %d z: %d", e_vec.x, e_vec.y, e_vec.z);
    send->fwrite(send, "Quat w: %f x: %f y: %f z: %f", q_vec.w, q_vec.x,
                 q_vec.y, q_vec.z);
    send->fwrite(send, "Temp: %d C", temp);
}