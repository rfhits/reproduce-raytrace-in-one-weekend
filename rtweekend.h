#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include<cstdlib>
#include <limits>
#include <memory>

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

const double pi = 3.1415926536;
const double infinity = std::numeric_limits<double>::infinity();

inline double degrees_to_radians(double degrees) {
    return degrees* pi / 180.0 ;
}

// generate random double in [0, 1)
inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

// generate random double in [min, max)
inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

inline double linear_to_gamma(double linear_component) {
    return sqrt(linear_component);
}

#include "ray.h"
#include"vec3.h"
#include "interval.h"


#endif