#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

using std::make_shared;
using std::shared_ptr;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.141592653589793285;

// Utility functions

inline double degrees_to_rads(double degrees)
{
    return degrees * pi / 180.0;
}

// common headers

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"