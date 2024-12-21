#ifndef MLEM
/******* This file will contain usefull constants and more. ********/
#define MLEM

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

using std::shared_ptr;
using std::make_shared;

// Utilityk
double degrees_to_radians(double deg){
    return deg * M_PI / 180.0;
}

double random_double() {
    // [0,1)
    return rand() / (RAND_MAX + 1.0);
}

double random_double(double min, double max) {
    // [min, max)
    return min + (max-min) * random_double();
}

// Constants

const double infinity = std::numeric_limits<double>::infinity();

// Headers

#include "ray.h"
#include "interval.h"

#endif
