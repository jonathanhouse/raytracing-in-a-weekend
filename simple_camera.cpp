#include "utils.h"

double hit_sphere(const point3& center, double radius, const ray& r){

    vec3 oc = center - r.origin();
    double a = r.direction().mag_squared();
    double h = dot(r.direction(), oc);
    double c = dot(oc,oc) - radius*radius;
    double discriminant = h*h - a*c;


    if(discriminant < 0){
        return -1.0;
    }
    else {
        return (h - std::sqrt(discriminant)) / a;
    }

}

color ray_color(const ray& r){

    double t = hit_sphere(point3(0,0,-1),0.5,r);
    if(t > 0.0){
        // normal as normalized vector pointing from center of sphere to intersection
        vec3 normal = unit_vector(r.at(t) - vec3(0,0,-1));
        return 0.5*color(normal.x()+1,normal.y()+1,normal.z()+1);
    }


    vec3 unit_direction = unit_vector(r.direction());
    double a = 0.5*(unit_direction.y() + 1.0); // y=[-1/2, 1/2]
    return (1-a)*color(1,1,1) + a*color(0.5,.7,1); // lerp between white and blue shade 
}

int main(){

    double aspect_ratio = 16.0/9.0; // width : height

    int image_width = 400;
    int image_height = int(image_width / aspect_ratio );
    image_height = (image_height < 1) ? 1 : image_height;

    double viewport_height = 2.0;
    double viewport_width = viewport_height * (double(image_width)/image_height); // use actual proportion rather than aspect_ratio

    point3 camera_center(0,0,0);
    double focal_length = 1; // distance between camera and image plane

    vec3 viewport_u(viewport_width, 0, 0);
    vec3 viewport_v(0,-viewport_height, 0);

    vec3 du = viewport_u / image_width;
    vec3 dv = viewport_v / image_height;

    // goes to edge of image plane in front of camera
    vec3 viewport_upper_left = camera_center - vec3(0,0,focal_length) - viewport_u/2 - viewport_v/2;

    vec3 pixel00_loc = viewport_upper_left + 0.5 * (du + dv); // pixels are in centers of du dv

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for(int row = 0; row < image_height; row++ ){
        // std::clog << "\rScanlines remaining: " << (image_height - row) << ' ' << std::flush; // 
        for(int col = 0; col < image_width; col++){

            if(row == image_height/2 && col == image_width/2){
                int x = 1;
            }
            point3 pixel_center = pixel00_loc + row*dv + col*du;
            vec3 ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);


        }
    }

    return 0;
}