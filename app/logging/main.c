
#include "logging.h"
#include "w25qlogger.h"
#include "mock_w25q.h"

#include <stdio.h>

Logger logger;
LogBuilder builder;
LogSubscriber sub[2];
W25q flash;
W25qLogger flash_log;

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
    printf("buildnew\n");
    return true;
}
const uint8_t * bgetptr(LogBuilder *builder)
{
    return "hello";
}

size_t bgetsize(LogBuilder *builder)
{
    return 5;
}

int main(int argc, char* argv[])
{
    SendInit(&send, logprint);
    sub[0].clear = subclear;
    sub[0].write = subwrite;
    sub[0].retrieve_all = subretrieve;
    MockW25qInit(&flash);
    W25qLoggerInit(&sub[1], &flash_log, &flash, 32);
    builder.build_new = bbuildnew;
    builder.get_ptr = bgetptr;
    builder.get_size = bgetsize;
    logger_init(&logger, &builder, sub, 2, &send);
    logger_update(&logger);
    logger_enable(&logger, true);
    logger_update(&logger);
    logger_update(&logger);
    logger_update(&logger);
    logger_update(&logger);
    logger_retrieve(&logger);
    return 0;
}