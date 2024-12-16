#ifndef HITTABLE_H
#define HITTABLE_H

#include "vec3.h"

struct hit_record {
    point3 p;
    vec3 normal;
    double t;
};

#endif
