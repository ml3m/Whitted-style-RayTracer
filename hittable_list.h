#ifndef SPHERE_LIST_H
#define SPHERE_LIST_H

#include <memory>
#include <vector>
#include "hittable.h"
#include "vec3.h"

using std::shared_ptr;

class hittable_list : public hittable {
    public:
        std::vector<shared_ptr<hittable>> objects;

    public:
        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) {
            add(object);
        }

        void clear() {
            objects.clear();
        }

        void add(shared_ptr<hittable> object) {
            objects.push_back(object);
        }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override;


};

bool hittable_list::hit(const ray& r, interval ray_t, hit_record& rec) const{
    bool hit_anything = false;
    auto closest_so_far = ray_t.max;

    for (const auto& object : objects) {
        if (object->hit(r, interval(ray_t.min, closest_so_far), rec)) {
            hit_anything = true; 
            closest_so_far = rec.t;
        }
    }
    return hit_anything;
}

#endif
