#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <cmath>
#include <iostream>

inline double liniar_to_gamma(double linear_component) {
    if (linear_component > 0)
        return std::sqrt(linear_component);

    return 0;
}

void write_color(std::ostream &out, color pixel_color) {
    // writes the transalted [0, 255] value of each color.
    static const interval intensity(0.000, 0.999);
    out << static_cast<int>(256 * intensity.clamp(liniar_to_gamma(pixel_color.x()))) << ' '
        << static_cast<int>(256 * intensity.clamp(liniar_to_gamma(pixel_color.y()))) << ' '
        << static_cast<int>(256 * intensity.clamp(liniar_to_gamma(pixel_color.z()))) << '\n';
}

#endif
