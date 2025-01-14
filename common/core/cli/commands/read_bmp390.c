
#include "read_bmp390.h"

#define APPROX_SEA_LEVEL_PA 101325.0

static Bmp390 *bmp;
static float start_press = 0;

void init_read_bmp390(Bmp390 * bmp_dev)
{
    bmp = bmp_dev;
    Bmp390_Config(bmp);
    start_press = bmp->get_pressure_pa(bmp);
}

void read_bmp390(int argc, char* argv[])
{
    Bmp390_Config(bmp);

    float press_pa = bmp->get_pressure_pa(bmp);
    float temp_c = bmp->get_temp_c(bmp);

    cli_write("Pressure: %f Pa, Temp: %f C", press_pa, temp_c);
    cli_write("Altitude: %f m", altitude(press_pa, start_press));
}
