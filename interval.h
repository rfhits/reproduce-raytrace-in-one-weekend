#ifndef INTERVAL_H
#define INTERVAL_H
#include "rtweekend.h"

class interval {
public:
    interval() : min(+infinity), max(-infinity) {}
    interval(double _min, double _max) : min(_min), max(_max) {}
    bool contains(double val) const {
        return min <= val && val <= max;
    }

    bool surrounds(double val) const {
        return min < val && val < max;
    }

    double clamp(double val) const {
        if (val < min) {
            return min;
        }
        if (val > max) {
            return max;
        }
        return val;
    }

    static const interval empty, universe;
public:
    double min, max;
};

static const interval empty();
static const interval universe(-infinity, +infinity);

#endif