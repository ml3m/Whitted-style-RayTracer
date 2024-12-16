#include <memory>
#include "hittable_list.h"
#include "camera.h"
#include "sphere.h"
#include "mlem.h"
#include "vec3.h"

using std::make_shared;

int main () {

    // world

    hittable_list world;
    // green
    world.add(make_shared<sphere>(point3(-1,0,-1),     0.5, color(0.2, 0.7, 0.1), true));
    // red
    world.add(make_shared<sphere>(point3(0,0,-1),      0.5, color(0.7, 0.2, 0.1), false));
    // blue
    world.add(make_shared<sphere>(point3(1,0,-1),      0.5, color(0.1, 0.2, 0.7), true));

    // on top
    world.add(make_shared<sphere>(
        point3(0, 0.9, 0.3),  // New center is above the original sphere
        0.2,                // Smaller radius
        color(0.2, 0.5, 0.8),// Different color for the smaller sphere
        false
    ));

    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100 ,color(0.5, 0.5, 0.5), false));


    // Camera

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 2000;
    cam.max_depth = 50; // max ray-bouncing.

    cam.render(world);
}
