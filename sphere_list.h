#ifndef SPHERE_LIST_H
#define SPHERE_LIST_H

#include <vector>
#include "hittable.h"
#include "sphere.h"

class sphere_list {
    public:
        std::vector<sphere> objects;

    public:
        sphere_list() {}
        sphere_list(sphere object) {
            add(object);
        }

        void clear() {
            objects.clear();
        }

        void add(sphere object) {
            objects.push_back(object);
        }

        bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const;

};

bool sphere_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const{
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object.hit(r, t_min, closest_so_far, rec)) {
            hit_anything = true; 
            closest_so_far = rec.t;
        }
    }
    return hit_anything;
}

#endif
