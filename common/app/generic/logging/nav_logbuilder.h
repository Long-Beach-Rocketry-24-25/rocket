
#pragma once

#include "logging.h"
#include "nav_data.h"

#include <string.h>

#define NAV_DATA_MAX_SIZE_BYTES 255

typedef struct
{
    const NavData * data;
    uint8_t buffer[NAV_DATA_MAX_SIZE_BYTES];
    size_t size;
} NavDataLogBuilder;

void NavDataLogBuilderInit(LogBuilder *builder, NavDataLogBuilder *nav_builder, const NavData *data);
bool NavDataLogBuilderBuildNew(LogBuilder *builder);
const uint8_t *NavDataLogBuilderGetPtr(LogBuilder *builder);
size_t NavDataLogBuilderGetSize(LogBuilder *builder);
