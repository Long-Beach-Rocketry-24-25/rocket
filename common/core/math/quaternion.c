
#include "quaternion.h"

bool quat_to_rotation_matrix(QuaternionVec* quat, Matrix* ro_mat)
{
    if (quat == NULL || ro_mat == NULL || ro_mat->rows != 3 ||
        ro_mat->cols != 3)
    {
        return false;
    }

    // First row of the rotation matrix
    MAT_GET(ro_mat, 0, 0) = 1 - 2 * quat->y * quat->y - 2 * quat->z * quat->z;
    MAT_GET(ro_mat, 0, 1) = 2 * quat->x * quat->y - 2 * quat->w * quat->z;
    MAT_GET(ro_mat, 0, 2) = 2 * quat->x * quat->z + 2 * quat->w * quat->y;

    // Second row of the rotation matrix
    MAT_GET(ro_mat, 1, 0) = 2 * quat->x * quat->y + 2 * quat->w * quat->z;
    MAT_GET(ro_mat, 1, 1) = 1 - 2 * quat->x * quat->x - 2 * quat->z * quat->z;
    MAT_GET(ro_mat, 1, 2) = 2 * quat->y * quat->z - 2 * quat->w * quat->x;

    // Third row of the rotation matrix
    MAT_GET(ro_mat, 2, 0) = 2 * quat->x * quat->z - 2 * quat->w * quat->y;
    MAT_GET(ro_mat, 2, 1) = 2 * quat->y * quat->z + 2 * quat->w * quat->x;
    MAT_GET(ro_mat, 2, 2) = 1 - 2 * quat->x * quat->x - 2 * quat->y * quat->y;

    return true;
}

QuaternionVec* quat_multiply(QuaternionVec* q1, QuaternionVec* q2,
                             QuaternionVec* out)
{
    out->w = q1->w * q2->w - q1->x * q2->x - q1->y * q2->y - q1->z * q2->z;
    out->x = q1->w * q2->x + q1->x * q2->w + q1->y * q2->z - q1->z * q2->y;
    out->y = q1->w * q2->y - q1->x * q2->z + q1->y * q2->w + q1->z * q2->x;
    out->z = q1->w * q2->z + q1->x * q2->y - q1->y * q2->x + q1->z * q2->w;

    return out;
}

QuaternionVec* quat_conjugate(QuaternionVec* quat, QuaternionVec* out)
{
    out->w = quat->w;
    out->x = -quat->x;
    out->y = -quat->y;
    out->z = -quat->z;

    return out;
}

bool quat_compare(QuaternionVec* q1, QuaternionVec* q2)
{
    return (q1->w == q2->w) && (q1->x == q2->x) && (q1->y == q2->y) &&
           (q1->z == q2->z);
}
