
#include "logging.h"
#include <stdio.h>

Logger logger;
LogBuilder builder;
LogSubscriber sub[1];

bool logprint(const char * msg)
{
    printf(msg);
    return true;
}

Send send;

bool subclear(LogSubscriber *sub)
{
    printf("sub clear\n");
    return true;
}

bool subwrite(LogSubscriber *sub, const uint8_t *data, size_t size)
{
    printf("sub write\n");
    return true;
}
bool subretrieve(LogSubscriber *sub, Send *sender)
{
    printf("sub retrieve\n");
    return true;
}

bool bbuildnew(LogBuilder *builder)
{
    return true;
}
const uint8_t * bgetptr(LogBuilder *builder)
{
    return "hello\n";
}

size_t bgetsize(LogBuilder *builder)
{
    return 0;
}

int main(int argc, char* argv[])
{
    SendInit(&send, logprint);
    sub[0].clear = subclear;
    sub[0].write = subwrite;
    sub[0].retrieve_all = subretrieve;
    builder.build_new = bbuildnew;
    builder.get_ptr = bgetptr;
    builder.get_size = bgetsize;
    logger_init(&logger, &builder, sub, 1, &send);
    logger_update(&logger);
    logger_enable(&logger, true);
    logger_update(&logger);
    logger_update(&logger);
    logger_update(&logger);
    logger_retrieve(&logger);
    return 0;
}