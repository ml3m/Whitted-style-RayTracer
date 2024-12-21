#ifndef MLEM
/******* This file will contain usefull constants and more. ********/
#define MLEM

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <random>  // Include the C++ random library

using std::shared_ptr;
using std::make_shared;

// Threads

thread_local std::mt19937 gen(std::random_device{}());
thread_local std::uniform_real_distribution<double> dist(0.0, 1.0);

// Utilityk
double degrees_to_radians(double deg){
    return deg * M_PI / 180.0;
}

double random_double(double min = 0.0, double max = 1.0) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

// Constants

const double infinity = std::numeric_limits<double>::infinity();

// Headers

#include "ray.h"
#include "interval.h"

#endif
