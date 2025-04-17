#ifndef RAY_H
#define RAY_H

#include "util.h"

class ray {
    private: 
        point3 orig;
        vec3 dir;

    public: 
        ray() : orig(), dir() {}
        ray(const ray& r) : orig(r.orig), dir(r.dir) {}
        ray(const point3& origin, vec3& direction) : orig(origin), dir(direction) {}

        const point3& origin() const { return orig; }
        const vec3& direction() const { return dir; }

        point3 at(double t) const { return orig + t*dir; }
        
};

#endif