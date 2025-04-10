
#include <math.h>
#include "acceleration.h"
#include "helios.h"
#include "helios_basic_cfg.h"
#include "matrix.h"
#include "mock_nav_data.h"
#include "nav_data.h"
#include "navigator.h"
#include "simple_kalman.h"

NavData nav_data;
MockNavData mock_nav;

Helios helios;
SimpleKalman* kalman;
Navigator nav;

int main(int argc, char* argv[])
{
    double sd_p = 2;
    double sd_a = 4;
    double t = 0.05;

    MockNavDataInit(&nav_data, &mock_nav, "./test_data.log");
    HeliosBasicConfig(&helios.kalman, sd_p, sd_a, t);
    HeliosNavigatorInit(&nav, &helios, &nav_data);

    if (!nav.start(&nav))
    {
        printf("Failed to start Nav Alg!\n");
        while (1);
    }

    double start_press = nav_data.pressure;

    for (size_t i = 0; i < (948 - 1); ++i)
    {
        bool success = nav.update(&nav);
        ThreeAxisVec raw_vert = {0};
        accel_rotate(&nav_data.accel, &nav_data.quat, &raw_vert);
        // Meas Alt, Meas Accel | Pred Alt, Pred Velo, Pred Accel
        printf("%f %f | %f %f %f\n", altitude(nav_data.pressure, start_press),
               raw_vert.z - EARTH_GRAVITY_M_S2, nav.altitude(&nav),
               nav.velocity(&nav), nav.acceleration(&nav));
    }

    return 0;
}