#pragma once

#include "vec3.h"

struct material
{
    double kd, ks, ka;
    vec3 od, os;
    double shininess;
};

struct light
{
    virtual vec3 direction_from(const point3& p) const = 0;
    virtual vec3 intensity() const = 0;
};