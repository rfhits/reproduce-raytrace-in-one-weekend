#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"

using color = vec3;


void write_color(std::ostream& os, const color& color, int samples_per_pixel)
{
    const double max_value = 256;
    static const interval intensity(0.0, 0.999);
    double scale = 1.0 / samples_per_pixel;
    auto scale_color = color * scale;

    os << (int)(intensity.clamp(linear_to_gamma(scale_color.x)) * max_value) << " "
        << (int)(intensity.clamp(linear_to_gamma(scale_color.y)) * max_value) << " "
        << (int)(intensity.clamp(linear_to_gamma(scale_color.z)) * max_value) << "\n";
}

#endif