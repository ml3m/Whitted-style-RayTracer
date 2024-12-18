#include <cmath>
#include <iostream>
#include "vec3.h"
#include "color.h"
#include "ray.h"

color getColor(const ray& r) {
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

double hit_sphere(const point3& center, double radius, const ray& r){
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto half_b = dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = half_b * half_b - a*c; 
    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-half_b - sqrt(discriminant)) / a;
    }
}

int main () {

    // Img
    
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera
    
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0,0,0);
    auto horizontal = vec3(viewport_width, 0,0);
    auto vertical = vec3(0,viewport_height,0);
    auto lower_left_corner = origin - horizontal/2  - vertical/2 - vec3(0,0,focal_length);

    // Render
    //
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "Scanlines remaining: " << j << '\n';
        for (int i = 0; i < image_width; ++i) {

            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);

            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);

            color pixel_color;
            point3 center(0,0,-1);
            auto t = hit_sphere(center, 0.5, r);

            if (t > 0.0) {
                vec3 n = unit_vector(r.at(t) - center);
                pixel_color = 0.5*color(n.x()+1, n.y()+1, n.z()+1);
            } else {
                pixel_color = getColor(r);
            }

            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}
