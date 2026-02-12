#pragma once

#include "hittable_list.h"

struct world
{
    hittable_list objects;
    std::vector<light> lights;
};