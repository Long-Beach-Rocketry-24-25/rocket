
#include "read_tmp102.h"

static Send* send;
static Tmp102* tmp;

void init_read_tmp102(Send* sender, Tmp102* tmp_dev)
{
    tmp = tmp_dev;
    send = sender;
}

void read_tmp102(int argc, char* argv[])
{
    float x = tmp->get_temp_c(tmp);
    send->fwrite(send, "Temp: %fC", x);
}