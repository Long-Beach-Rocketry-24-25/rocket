
#include "logging.h"
#include "w25q_logger.h"
#include "mock_w25q.h"

#include <stdio.h>

Logger logger;
LogBuilder builder;
LogSubscriber sub;
W25q flash;
W25qLogger flash_log;

bool logprint(const char * msg)
{
    printf(msg);
    return true;
}

Send send;

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
    static size_t i = 0;
    i++;
    i %= 5;
    return i;
}

int main(int argc, char* argv[])
{
    SendInit(&send, logprint);
    MockW25qInit(&flash);
    W25qLoggerInit(&sub, &flash_log, &flash, flash.mem_size / flash.page_size);
    W25qLoggerWrapAround(&sub, true);
    builder.build_new = bbuildnew;
    builder.get_ptr = bgetptr;
    builder.get_size = bgetsize;
    logger_init(&logger, &builder, &sub, 1, &send);
    logger_update(&logger);
    logger_enable(&logger, true);
    logger_update(&logger);
    for (size_t i = 0; i < 18; ++i)
    {
        logger_update(&logger);
        // MockW25qDumpMem(&send);
    }
    logger_retrieve(&logger);
    return 0;
}
