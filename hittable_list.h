#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
public:
    std::vector<shared_ptr<hittable>> objects;
    hittable_list() {};
    hittable_list(shared_ptr<hittable> object) {
        add(object);
    }
    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
    }

    bool hit(const ray& r, const interval& ray_t, hit_record& rec) const override {
        // for each obj
        //   update tmin and tmax
        //   base on tmin and tmax update rec
        bool once_hit = false;
        auto close_tmax = ray_t.max;
        hit_record temp_rec;

        for (const auto& object : objects) {
            bool b_hit = object->hit(r, interval(ray_t.min, close_tmax), temp_rec);
            once_hit |= b_hit;
            if (b_hit) {
                close_tmax = temp_rec.t;
                rec = temp_rec;
            }
        }
        return once_hit;
    }

};

#endif