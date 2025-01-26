
#include "logging.h"
#include "w25q_logger.h"
#include "mock_w25q.h"
#include "mock_nav_data.h"
#include "nav_logbuilder.h"

#include <stdio.h>

Logger logger;
LogBuilder builder;
LogSubscriber sub;
W25q flash;
W25qLogger flash_log;
NavDataLogBuilder nav_builder;
NavData nav;
MockNavData mock_nav;

bool logprint(const char * msg)
{
    printf("%s\n", msg);
    return true;
}

Send send;

int main(int argc, char* argv[])
{
    SendInit(&send, logprint);
    MockW25qInit(&flash);
    W25qLoggerInit(&sub, &flash_log, &flash, flash.mem_size / flash.page_size);
    W25qLoggerWrapAround(&sub, true);
    MockNavDataInit(&nav, &mock_nav, "./navdata.log");
    NavDataLogBuilderInit(&builder, &nav_builder, &nav);
    logger_init(&logger, &builder, &sub, 1, &send);
    logger_update(&logger);
    logger_enable(&logger, true);
    logger_update(&logger);
    for (size_t i = 0; i < 18; ++i)
    {
        nav.update(&nav);
        logger_update(&logger);
        // MockW25qDumpMem(&send);
    }
    logger_retrieve(&logger);
    MockNavDataDeinit(&nav);
    return 0;
}
