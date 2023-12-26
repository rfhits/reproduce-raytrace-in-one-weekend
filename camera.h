#ifndef CAMERA_H
#define CAMERA_H

#include"rtweekend.h"
#include"color.h"
#include"hittable.h"
#include "hittable_list.h"
#include <iostream>


class camera {
public:
    camera() {};

    void render(const hittable_list& world) {
        initialize();
        // Render
        std::cout << "P3\n" << image_width << " " << image_height << "\n255\n" << std::endl;
        for (int j = 0; j < image_height; ++j)
        {
            std::clog << "\rScanLines remains: " << image_height - j << "       " << std::flush;
            for (int i = 0; i < image_width; ++i)
            {
                color sum_color;
                for (int sample = 0; sample < sample_cnt; ++sample) {
                    ray sample_ray = get_ray(i, j);
                    color pixel_color = ray_color(sample_ray, max_depth, world);
                    sum_color += pixel_color;
                }


                write_color(std::cout, sum_color, sample_cnt);
            }
        }
        std::clog << "\rDone                    " << std::endl;
    }

public:
    double aspect_ratio;
    int image_width; // number of pixels in a line
    point3 lookfrom{ 0,0,-1 };
    point3 lookat{ 0,0,0 };
    vec3 vup{ 0,1,0 };
    double fov = 179;

    double defocus_angle = 0;
    double focus_dist = 1;

private:
    point3 pixel00_loc;
    vec3 pixel_delta_u, pixel_delta_v;
    vec3 defocus_disk_u, defocus_disk_v;
    vec3 u, v, w;

    int sample_cnt = 10;
    int max_depth = 10;
    int image_height;

    void initialize() {
        image_height = image_width / aspect_ratio;
        // std::clog << image_width << " " << aspect_ratio << std::endl;
        image_height = (image_height > 1) ? image_height : 1;
        // std::clog << image_height << std::endl;

        auto center = lookfrom;

        // viewport
        double focal_length = focus_dist;
        auto theta = degrees_to_radians(fov / 2);
        double viewport_height = tan(theta / 2) * focal_length * 2;
        double viewport_width = viewport_height * (double)(image_width) / image_height;

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        auto viewport_u = u * viewport_width;
        auto viewport_v = -v * viewport_height;

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        auto viewport_upper_left = center - w * focal_length - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        // lens
        double defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = defocus_radius * u;
        defocus_disk_v = defocus_radius * v;

    }

    // generate a random ray hit pixel i, j
    ray get_ray(int i, int j) {
        double px = random_double();
        double py = random_double();
        point3 pixel_sample = pixel00_loc + (i - 0.5 + px) * pixel_delta_u + (j - 0.5 + py) * pixel_delta_v;
        point3 ray_origin = (defocus_angle > 0) ? defocus_disk_sample() : lookfrom;
        auto ray_dir = pixel_sample - ray_origin;
        return ray(ray_origin, ray_dir);
    }

    point3 defocus_disk_sample() {
        auto rv = random_in_unit_disk();
        return lookfrom + rv.x * defocus_disk_u + rv.y * defocus_disk_v;
    }

    color ray_color(const ray& r, int depth, const hittable_list& world) const {
        if (depth <= 0) {
            return color(0, 0, 0);
        }
        hit_record rec;
        if (world.hit(r, interval(0.001, infinity), rec)) {
            color  attenuation;
            ray scattered;
            if (rec.mat->scatter(r, rec, attenuation, scattered)) {
                return attenuation * ray_color(scattered, depth - 1, world);
            }
            else {
                return color(0, 0, 0);
            }
        }
        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }
};

#endif