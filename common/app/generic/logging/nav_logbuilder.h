
#pragma once

#include "logging.h"
#include "nav_data.h"

typedef struct
{
    const NavData * data;
} NavDataLogBuilder;

NavDataLogBuilderInit(LogBuilder *builder, NavDataLogBuilder *nav_builder, const NavData *data);
NavDataLogBuilderBuildNew(LogBuilder *builder);
NavDataLogBuilderGetPtr(LogBuilder *builder);
NavDataLogBuilderGetSize(LogBuilder *builder);

