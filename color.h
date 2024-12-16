#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <cmath>
#include <iostream>

double liniar_to_gamma(double liniar_component) {
    return sqrt(liniar_component);
}

void write_color(std::ostream &out, color pixel_color) {
    // writes the transalted [0, 255] value of each color.
    static const interval intensity(0.000, 0.999);
    out << static_cast<int>(255 * liniar_to_gamma(pixel_color.x())) << ' '
        << static_cast<int>(255 * liniar_to_gamma(pixel_color.y())) << ' '
        << static_cast<int>(255 * liniar_to_gamma(pixel_color.z())) << '\n';
}

#endif
