
#pragma once

#include "nav_data.h"

#include <stdio.h>
#include <string.h>

typedef struct
{
    FILE *fp;
} MockNavData;

bool MockNavDataInit(NavData *data, MockNavData *mock, const char *path);
void MockNavDataDeinit(NavData *data);
bool MockNavDataUpdate(NavData *data);