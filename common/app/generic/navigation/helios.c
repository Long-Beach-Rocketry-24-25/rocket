
#include "helios.h"

bool HeliosNavigatorInit(Navigator* nav, Helios* helios, NavData* nav_data)
{
    if (nav == NULL || helios == NULL || nav_data == NULL ||
        nav_data->update == NULL)
    {
        return false;
    }

    helios->data = nav_data;
    helios->started = false;
    nav->priv = (void*)helios;

    nav->altitude = HeliosAltitude;
    nav->orientation = HeliosOrientation;
    nav->start = HeliosStart;
    nav->update = HeliosUpdate;
    nav->velocity = HeliosVelocity;

    return true;
}

// bool HeliosKalmanInit(Matrix* A, Matrix* H, double timestep)
// {
//     if (A == NULL || H == NULL)
//     {
//         return false;
//     }

//     MATRIX(A_dat, 4, 4, {1, timestep, timestep * timestep / 2, 0},
//            {0, 1, timestep, 0}, {0, 0, 1, 0}, {0, 0, 0, 1});
//     MATRIX(H_dat, 2, 4, {1, 0, 0, 0}, {0, 0, 1, 1});
//     return matrix_copy(&A_dat, A) != NULL && matrix_copy(&H_dat, H) != NULL;
// }

bool HeliosConfig(Navigator* nav, Matrix* x, Matrix* P, Matrix* A, Matrix* H,
                  Matrix* Q, Matrix* R)
{
    if (nav == NULL || x == NULL || P == NULL || A == NULL || H == NULL ||
        Q == NULL || R == NULL)
    {
        return false;
    }

    Helios* p = (Helios*)nav->priv;

    p->kalman.x = x;
    p->kalman.P = P;
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

    if (p->kalman.model.A == NULL || p->kalman.model.H == NULL ||
        p->kalman.model.Q == NULL || p->kalman.model.R == NULL ||
        p->kalman.x == NULL || p->kalman.P == NULL)
    {
        return false;
    }

    if (!p->data->update(p->data))
    {
        return false;
    }

    p->base_pressure = p->data->pressure;
    p->started = true;

    return true;
}

bool HeliosUpdate(Navigator* nav)
{
    if (nav == NULL)
    {
        return false;
    }

    Helios* p = (Helios*)nav->priv;

    if (!p->started)
    {
        return false;
    }

    static size_t state = 0;

    if (state++ < 1)
    {
        return SimpleKalmanPredict(&p->kalman);
    }
    else
    {
        state = 0;
        if (!SimpleKalmanPredict(&p->kalman))
        {
            return false;
        }
        p->data->update(p->data);

        // Use starting pressure as sea level to measure relative altitude.
        double curr_altitude = altitude(p->data->pressure, p->base_pressure);

        // Calculate velocity from last velocity (Kalman x[1][0]), current vertical accel, and timestep (Kalman A[0][1]).
        ThreeAxisVec corrected_accel;
        accel_rotate(&p->data->accel, &p->data->quat, &corrected_accel);
        double curr_vert_accel = corrected_accel.z - EARTH_GRAVITY_M_S2;

        printf("%f %f | ", curr_altitude, curr_vert_accel);

        static double last_vert_acc = 0;
        static double last_alt = 0;
#define HEL_K_ALPHA 0.5
        curr_vert_accel =
            HEL_K_ALPHA * curr_vert_accel + ((1 - HEL_K_ALPHA) * last_vert_acc);

        // #define HEL_ALT_ALPH 0.3
        //         curr_altitude =
        //             HEL_ALT_ALPH * curr_altitude + ((1 - HEL_ALT_ALPH) * last_alt);

        // Update Kalman filter.
        MATRIX(z, 2, 1, {curr_altitude}, {curr_vert_accel});
        bool s = SimpleKalmanEstimate(&p->kalman, &z);

        printf("%f %f %f %f\n", MAT_GET(p->kalman.x, 0, 0),
               MAT_GET(p->kalman.x, 1, 0), MAT_GET(p->kalman.x, 2, 0),
               MAT_GET(p->kalman.x, 3, 0));
        last_vert_acc = curr_vert_accel;
        last_alt = curr_altitude;
        return s;
    }

    return true;
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