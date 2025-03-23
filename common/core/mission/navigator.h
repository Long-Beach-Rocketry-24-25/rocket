
#pragma once

typedef struct Navigator Navigator;

typedef struct
{
    double (*altitude)(void);
    double (*velocity)(void);
    void* priv;
} Navigator;
