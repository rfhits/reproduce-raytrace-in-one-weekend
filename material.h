#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"
#include "vec3.h"
#include "color.h"
#include "hittable.h"



class material
{
private:

public:
    virtual ~material() = default;

    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};


class lambertian : public material {
public:
    lambertian(const color& c) : albedo(c) {};

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        auto scatter_direction = rec.normal + random_unit_vec();
        scattered = ray(rec.p, scatter_direction);
        if (scatter_direction.near_zero()) {
            scatter_direction = rec.normal;
        }
        attenuation = albedo;
        return true;
    }

private:
    color albedo;
};

class metal : public material {
public:
    metal(const color& c, double f) : albedo(c), fuzz(f < 1 ? f : 1) {};
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        auto reflect_dir = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflect_dir + fuzz * random_unit_vec());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

private:
    color albedo;
    double fuzz;
};

class dielectric : public material {
public:
    dielectric(double _ir) : ir(_ir) {};
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        attenuation = color(1,1,1);
        double reflect_ratio = rec.front_face? 1/ir: ir;
        vec3 unit_in_direction = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-unit_in_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1-cos_theta*cos_theta);

        bool cannot_reflect = reflect_ratio * sin_theta > 1;
        vec3 direction;
        if (cannot_reflect) {
            direction = reflect(unit_in_direction, rec.normal);
        } else {
            direction = refract(unit_in_direction, rec.normal, reflect_ratio);
        }
        scattered = ray(rec.p, direction);
        return true;
    }

private:
    double ir;
};
#endif