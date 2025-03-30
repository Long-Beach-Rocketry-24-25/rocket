
#pragma once

typedef struct Navigator Navigator;

struct Navigator
{
    double (*altitude)(Navigator* nav);
    double (*velocity)(Navigator* nav);
    const QuaternionVec* (*orientation)(Navigator* nav);
    bool (*start)(Navigator* nav);
    bool (*update)(Navigator* nav);
    void* priv;
};
