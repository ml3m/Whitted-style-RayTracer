#ifndef HITTABLE_H
#define HITTABLE_H

#include "interval.h"
#include "vec3.h"
#include "ray.h"

class hit_record {
    public:
        point3 p;
        vec3 normal;
        double t;

        color attenuation;
        ray   scattered;
};

class hittable {
    public:
        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif

