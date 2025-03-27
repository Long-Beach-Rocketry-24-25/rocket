
#pragma once

typedef struct Navigator Navigator;

typedef struct
{
    double (*altitude)(Navigator* nav);
    double (*velocity)(Navigator* nav);
    bool (*reconfig)(Navigator* nav);
    bool (*update)(Navigator* nav);
    void* priv;
} Navigator;
