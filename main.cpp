#include <memory>
#include "hittable_list.h"
#include "camera.h"
#include "sphere.h"
#include "mlem.h"
#include "vec3.h"
#include "material.h"

using std::make_shared;

int main () {

    // world

    auto material_ground = make_shared<lambertian>(color(0.9, 0.6, 0.7));
    auto material_center = make_shared<lambertian>(color(0.7, 0.2, 0.1));
    auto material_left   = make_shared<metal>(color(0.2, 0.7, 0.1), 0);
    auto material_right  = make_shared<dielectric>(1.5);
    auto material_top    = make_shared<metal>(color(1,1,1));

    hittable_list world;

    world.add(make_shared<sphere>(point3(-1,0,-1),     0.5, material_left));
    world.add(make_shared<sphere>(point3(0,0,-1),      0.5, material_center));
    world.add(make_shared<sphere>(point3(1,0,-1),      0.5, material_right));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100, material_ground));
    world.add(make_shared<sphere>(point3(0,1.5,2),       2, material_top));


    // Camera

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1000;
    cam.max_depth = 50; // max ray-bouncing.

    cam.render(world);
}
