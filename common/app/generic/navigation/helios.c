
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
    p->p = 0;
    p->v = 0;
    p->a = 0;
    p->t = 0.050;
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

    if (!p->data->update(p->data))
    {
        return false;
    }

    static double last_p = 0;

    // Use starting pressure as sea level to measure relative altitude.
    double meas_p = altitude(p->data->pressure, p->base_pressure);

    double f_p = last_p * 0.99 + (meas_p * 0.01);

    ThreeAxisVec corrected_accel;
    accel_rotate(&p->data->accel, &p->data->quat, &corrected_accel);
    p->a = corrected_accel.z - EARTH_GRAVITY_M_S2 + 1.1;
    p->a = (fabs(p->a) < 0.45) ? 0 : p->a;
    // p->v = (fabs(f_p - last_p) < 1) ? 0 : p->v;
    // printf("%f %f\n\n", f_p, last_p);

    double a_v = (p->v + (p->t * p->a));
    // double p_v = (meas_p - last_p) * p->t;
    p->v = a_v;  // (0.9 * a_v) + (0.1 * p_v);
    last_p = p->p;

    p->p = (0.5 * meas_p) + (0.5 * (p->p + (p->t * p->v)));

    printf("%f %f %f | %f %f\n", meas_p, p->a, a_v, p->p, p->v);

    return true;
}

double HeliosAltitude(Navigator* nav)
{
    Helios* p = (Helios*)nav->priv;
    return p->p;
}

double HeliosVelocity(Navigator* nav)
{
    Helios* p = (Helios*)nav->priv;
    return p->v;
}

const QuaternionVec* HeliosOrientation(Navigator* nav)
{
    Helios* p = (Helios*)nav->priv;
    return (const QuaternionVec*)&p->data->quat;
}