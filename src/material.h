#pragma once

#include "color.h"
#include "vec3.h"

struct material
{
    double kd, ks, ka;
    vec3 od, os;
    double shininess;
};

struct light
{
    vec3 light_direction;
    color light_color;
    color ambient_light;
};