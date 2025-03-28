
#include "helios.h"

bool HeliosNavigatorInit(Navigator* nav, Helios* helios, NavData* nav_data)
{
    if (nav == NULL || helios == NULL || nav_data == NULL ||
        nav_data->update == NULL)
    {
        return false;
    }

    helios->data = nav_data;
    nav->priv = (void*)helios;

    return true;
}

bool HeliosKalmanInit(Matrix* A, Matrix* H, double timestep)
{
    if (A == NULL || H == NULL)
    {
        return false;
    }

    MATRIX(A_dat, 2, 2, {1, timestep}, {0, 1});
    MATRIX(H_dat, 2, 2, {1, 0}, {0, 0});
    return matrix_copy(A, &A_dat) != NULL && matrix_copy(H, &H_dat) != NULL;
}

bool HeliosConfig(Navigator* nav, Matrix* A, Matrix* H, Matrix* Q, Matrix* R)
{
    if (nav == NULL || A == NULL || H == NULL || Q == NULL || R == NULL)
    {
        return false;
    }

    Helios* p = (Helios*)nav->priv;

    p->kalman.model.A = A;
    p->kalman.model.H = H;
    p->kalman.model.Q = Q;
    p->kalman.model.R = R;

    return true;
}

bool HeliosStart(Navigator* nav)
{
    if (nav == NULL)
    {
        return false;
    }

    Helios* p = (Helios*)nav->priv;

    if (!p->data->update(p->data))
    {
        return false;
    }

    p->base_pressure = p->data->pressure;

    return true;
}

bool HeliosUpdate(Navigator* nav)
{
    if (nav == NULL)
    {
        return false;
    }

    Helios* p = (Helios*)nav->priv;
    p->data->update(p->data);

    // Use starting pressure as sea level to measure relative altitude.
    double curr_altitude = altitude(p->data->pressure, p->base_pressure);

    // Calculate velocity from last velocity (Kalman x[1][0]), current vertical accel, and timestep (Kalman A[0][1]).
    ThreeAxisVec corrected_accel;
    accel_rotate(&p->data->accel, &p->data->quat, &corrected_accel);
    double curr_velocity = accel_to_velocity(
        MAT_GET(p->kalman.x, 1, 0), corrected_accel.z - EARTH_GRAVITY_M_S2,
        MAT_GET(p->kalman.model.A, 0, 1));

    // Update Kalman filter.
    MATRIX(z, 2, 1, {curr_altitude}, {curr_velocity});

    return SimpleKalmanPredict(&p->kalman) &&
           SimpleKalmanEstimate(&p->kalman, &z);
}

double HeliosAltitude(Navigator* nav)
{
    Helios* p = (Helios*)nav->priv;
    return MAT_GET(p->kalman.x, 0, 0);
}

double HeliosVelocity(Navigator* nav)
{
    Helios* p = (Helios*)nav->priv;
    return MAT_GET(p->kalman.x, 1, 0);
}

const QuaternionVec* HeliosOrientation(Navigator* nav)
{
    Helios* p = (Helios*)nav->priv;
    return (const QuaternionVec*)&p->data->quat;
}