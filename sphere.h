#ifndef SPHERE_H
#define SPHERE_H

#include "utils.h"
#include "hittable.h"

class sphere : public hittable {

    private: 
        point3 center;
        double radius;
        shared_ptr<material> mat;

    public: 
        sphere(const point3& center, double radius, shared_ptr<material> mat) : 
                center(center), radius(std::fmax(0,radius)), mat(mat) {}

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {

            vec3 oc = center - r.origin();
            double a = r.direction().mag_squared();
            double h = dot(r.direction(), oc);
            double c = dot(oc,oc) - radius*radius;
            double discriminant = h*h - a*c;

            if(discriminant < 0){
                return false;
            }

            double sqrtd = std::sqrt(discriminant);
            double root = (h - sqrtd) / a;
            if(!ray_t.surrounds(root)){ // check if either root is in bounds of ray 
                root = (h + sqrtd) / a;
                if(!ray_t.surrounds(root)){ return false; }
            }

            rec.t = root;
            rec.p = r.at(root);
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat = mat;

            return true;
        }
};



#endif