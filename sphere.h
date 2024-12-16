#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "ray.h"
#include "vec3.h"

class sphere {
    public:
        point3 center;
        double radius;

    public:
        sphere() {}
        sphere(point3 cen, double r) : center(cen), radius(r) {};

        bool hit (const ray& r, double t_min, double t_max, hit_record& rec) const;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto half_b = dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;

    auto discriminant = half_b * half_b - a*c; 
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);


    // find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    rec.normal = (rec.p - center) / radius;

    return true;
}

#endif // !SPHERE_H
