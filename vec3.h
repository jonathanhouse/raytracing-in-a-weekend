#ifndef VEC3_H
#define VEC3_H

#include "utils.h"

class vec3 {

    private: 
        double e[3];

    public: 
    vec3() {e[0] = 0; e[1] = 0; e[2] = 0;}
    vec3(double e0, double e1, double e2) {
        e[0] = e0;
        e[1] = e1;
        e[2] = e2;
    }

    vec3(const vec3& v){
        e[0] = v.x();
        e[1] = v.y();
        e[2] = v.z();
    }

    vec3& operator=(const vec3& lhs){
        e[0] = lhs.x();
        e[1] = lhs.y();
        e[2] = lhs.z();
        return *this;
    }

    double x() const {return e[0];}
    double y() const {return e[1];}
    double z() const {return e[2];}

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const {return e[i];}
    double& operator[](int i) {return e[i];}

    vec3& operator+=(const vec3 &rhs){ 
        e[0] += rhs.x();
        e[1] += rhs.y();
        e[2] += rhs.z();
        return *this;
    }

    vec3& operator*=(double c){
        e[0] *= c;
        e[1] *= c;
        e[2] *= c;
        return *this;
    }

    vec3& operator/=(double c){
        return *this *= 1/c;
    }

    double mag_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    double mag() const {
        return std::sqrt(mag_squared());
    }

    static vec3 random(){
        return vec3(random_double(), random_double(), random_double());
    }

    static vec3 random(double min, double max){
        return vec3(random_double(min,max),random_double(min,max),random_double(min,max));
    }

    bool near_zero() const {
        double s = 1e-8;
        return (std::fabs(e[0] < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s));
    }


};

using point3 = vec3;

inline vec3 operator+(const vec3& lhs, const vec3& rhs){
    return vec3(lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z());
}
inline vec3 operator-(const vec3& lhs, const vec3& rhs){
    return vec3(lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z());
}
inline vec3 operator*(const vec3& lhs, const vec3& rhs){
    return vec3(lhs.x() * rhs.x(), lhs.y() * rhs.y(), lhs.z() * rhs.z());
}
inline vec3 operator*(double t, const vec3& rhs){
    return vec3(t*rhs.x(), t*rhs.y(), t*rhs.z());
}
inline vec3 operator*(const vec3& lhs,double t){
    return t*lhs;
}
inline vec3 operator/(const vec3& lhs, double t){
    return (1/t)*lhs;
}
inline double dot(const vec3& lhs, const vec3& rhs){
    return lhs.x()*rhs.x() + lhs.y()*rhs.y() + lhs.z()*rhs.z();
}
inline vec3 cross(const vec3& lhs, const vec3& rhs){
    return vec3(lhs.y()*rhs.z() - lhs.z()*rhs.y(),
                lhs.z()*rhs.x() - lhs.x()*rhs.z(),
                lhs.x()*rhs.y() - lhs.y()*rhs.x());
}

inline vec3 unit_vector(const vec3& v){
    return v / v.mag();
}

inline vec3 random_in_unit_disk() {
    while(true){
        vec3 p = vec3(random_double(-1,1), random_double(-1,1),0);
        if(p.mag_squared() , 1)
            return p;
    }
}

inline std::ostream& operator<<(std::ostream& out, const vec3& v){
    return out << v.x() << " " << v.y() << " " << v.z();
}

inline vec3 random_unit_vector(){
    while(true){
        vec3 r = vec3::random();
        double r_mag2 = r.mag_squared();
        if(r_mag2 <= 1 && r_mag2 > 1e-160){
            return r / sqrt(r_mag2);
        }
    }
}

inline vec3 random_on_hemisphere(const vec3& normal){
    vec3 on_unit_sphere = random_unit_vector();
    if(dot(on_unit_sphere,normal) > 0){ // they're in the same hemisphere
        return on_unit_sphere;
    }
    else {
        return -on_unit_sphere;
    }
}

inline vec3 reflect(const vec3& v, const vec3& normal){
    return v - 2*dot(v,normal)*normal;
}

inline vec3 refract(const vec3& uv, const vec3& normal, double eta_i_over_eta_t){
    double cos_theta = std::fmin(dot(-uv,normal),1.0);
    vec3 r_out_perp = eta_i_over_eta_t * (uv + cos_theta*normal);
    vec3 r_out_prl = -std::sqrt(std::fabs(1.0 - r_out_perp.mag_squared())) * normal;
    return r_out_perp + r_out_prl;

}


#endif 