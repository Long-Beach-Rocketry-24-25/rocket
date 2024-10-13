
#include "read_tmp102.h"

static Tmp102 *tmp;

void init_read_tmp102(Tmp102 * tmp_dev)
{
    tmp = tmp_dev;
}

void read_tmp102(int argc, char* argv[])
{
    float x = tmp->get_temp_c(tmp);
    cli_write("Temp: %fC", x);
}