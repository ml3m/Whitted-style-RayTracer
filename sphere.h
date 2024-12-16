#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "ray.h"
#include "vec3.h"
#include <functional>

class sphere : public hittable {
    public:
        point3 center;
        double radius;
        color  albedo;
        bool    metal;

    public:
        sphere() {}
        sphere(point3 cen, double r, color c, bool m) : center(cen), radius(r), albedo(c), metal(m){};

        bool hit (const ray& r, interval ray_t, hit_record& rec) const override;
};

bool sphere::hit(const ray& r, interval ray_t, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto half_b = dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;

    auto discriminant = half_b * half_b - a*c; 
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);


    // find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (!ray_t.surrounds(root)) {
        return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    rec.normal = (rec.p - center) / radius;

    rec.attenuation = albedo;

    vec3 direction;

    if (metal) {
        direction = reflect(r.direction(), rec.normal);
    } else {
        direction = rec.normal + random_in_unit_sphere();
    }


    rec.scattered = ray(rec.p, direction);

    return true;
}

#endif // !SPHERE_H
