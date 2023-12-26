#ifndef VEC3_H
#define VEC3_H

#include "rtweekend.h"
#include<iostream>
#include<cmath>

class vec3
{
public:
    union {
        struct {
            double x;
            double y;
            double z;
        };
        double e[3];
    };
    vec3() :e{ 0,0,0 } {}
    vec3(double x, double y, double z) : e{ x, y, z } {};

    vec3 operator-() const {
        return vec3(-x, -y, -z);
    }

    double operator[] (int i) const
    {
        return e[i];
    }

    double& operator[] (int i)
    {
        return e[i];
    }

    vec3 operator+(const vec3& b) const {
        return vec3(x + b.x, y + b.y, z + b.z);
    }

    vec3 operator-(const vec3& o) {
        return *this + (-o);
    }

    vec3& operator+=(const vec3& b) {
        x += b.x;
        y += b.y;
        z += b.z;
        return *this;
    }

    vec3& operator*=(double t) {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    vec3& operator/=(double t) {
        return (*this) *= (1 / t);
    }

    double length_squared() const {
        return x * x + y * y + z * z;
    }

    double length() const {
        return sqrt(length_squared());
    }

    double near_zero() const {
        double s = 1e-8;
        return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
    }

    static vec3 random();
    static vec3 random(double, double);

};

using point3 = vec3;

inline std::ostream& operator<<(std::ostream& o, const vec3& v) {
    o << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return o;
}

inline vec3 operator*(const vec3& v, double t) {
    return vec3(v.x * t, v.y * t, v.z * t);
}

inline vec3 operator*(double t, const vec3& v)
{
    return v * t;
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

inline vec3 operator/(const vec3& v, double t)
{
    return v * (1 / t);
}

inline double dot(const vec3& a, const vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline vec3 cross(const vec3& a, const vec3& b) {
    return vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );

}

inline vec3 unit_vector(const vec3& v)
{
    return v / v.length();
}


// a random vector in cube(min, max)
inline vec3 vec3::random(double min, double max) {
    return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

// a random vector in unit cube
inline vec3 vec3::random() {
    return random(-1, 1);
}

inline vec3 random_in_unit_sphere() {
    while (true) {
        vec3 v = vec3::random();
        if (v.length() > 1) {
            continue;
        }
        return v;
    }
}

inline vec3 random_in_unit_disk() {
    while (true) {
        vec3 v{ random_double(), random_double(), 0 };
        if (v.length() > 1) {
            continue;
        }
        return v;
    }
}

// random vector length = 1
inline vec3 random_unit_vec() {
    return unit_vector(random_in_unit_sphere());
}

inline vec3 random_on_hemisphere(const vec3& normal) {
    auto v = random_unit_vec();
    if (dot(v, normal) > 0.0) {
        return v;
    }
    return -v;
}

inline vec3 reflect(const vec3& in, const vec3& n) {
    return dot(-in, n) * 2 * n + in;
}


// copied, snell rule
inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

#endif