#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class camera {

    public: 
        double aspect_ratio = 1.0;
        int image_width = 100;
        int samples_per_pixel = 10; // count of random samples for each pixel
        int max_depth = 10; // max number of ray bounces in the scene
        double vfov; // vertical view angle (fov)
        point3 lookfrom = point3(0,0,0);
        point3 lookat = point3(0,0,-1);
        vec3 vup = vec3(0,1,0); //  "up" direction relative to the camera
        double defocus_angle = 0; // variation angle of rays 
        double focus_dis = 10; // distance from camera lookfrom point to plane of perfect focus

        void render(const hittable& world){

            initialize();
            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
            for(int row = 0; row < image_height; row++ ){
                // std::clog << "\rScanlines remaining: " << (image_height - row) << ' ' << std::flush; // 
                for(int col = 0; col < image_width; col++){

                    // if(row == image_height/2 && col == image_width/2){
                    //     int x = 1;
                    // }
                    color pixel_color(0,0,0);
                    for(int sample = 0; sample < samples_per_pixel; sample++){
                        ray r = get_ray(col,row);
                        pixel_color += ray_color(r,max_depth, world);
                    }

                    write_color(std::cout, pixel_samples_scale * pixel_color);
                }
            }   

        }
    
    private: 
        int image_height;
        double pixel_samples_scale;
        point3 center;
        point3 pixel00_loc;
        vec3 du;
        vec3 dv;
        vec3 u,v,w; // camera basis vectors 
        vec3 defocus_disk_u;
        vec3 defocus_disk_v;

        void initialize(){
            image_height = int(image_width / aspect_ratio );
            image_height = (image_height < 1) ? 1 : image_height;

            double theta = degrees_to_radians(vfov);
            double h = std::tan(theta/2);


            double viewport_height = 2.0 * h * focus_dis;
            double viewport_width = viewport_height * (double(image_width)/image_height); // use actual proportion rather than aspect_ratio
            pixel_samples_scale = 1.0 / samples_per_pixel;

            center = lookfrom;

            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup,w));
            v = cross(w,u);

            vec3 viewport_u(viewport_width * u);
            vec3 viewport_v(-viewport_height * v);

            du = viewport_u / image_width;
            dv = viewport_v / image_height;

            // goes to edge of image plane in front of camera
            vec3 viewport_upper_left = center - vec3(focus_dis * w) - viewport_u/2 - viewport_v/2;

            pixel00_loc = viewport_upper_left + 0.5 * (du + dv); // pixels are in centers of du dv

            double defocus_radius = focus_dis * std::tan(degrees_to_radians(defocus_angle / 2));
            defocus_disk_u = u * defocus_radius;
            defocus_disk_v = v * defocus_radius;

        }

        color ray_color(const ray& r, int depth, const hittable& world) const {
            if(depth <= 0){
                return color(0,0,0);
            }

            hit_record rec;
            if(world.hit(r, interval(0.001,infinity), rec)){
                ray scattered;
                color attenuation;
                if(rec.mat->scatter(r,rec,attenuation,scattered))
                    return attenuation * ray_color(scattered, depth-1, world);
                return color(0,0,0);
            }


            vec3 unit_direction = unit_vector(r.direction());
            double a = 0.5*(unit_direction.y() + 1.0); // y=[-1/2, 1/2]
            return (1-a)*color(1,1,1) + a*color(0.5,.7,1); // lerp between white and blue shade 
        }

        ray get_ray(int i,int j) const {
            vec3 offset = sample_square();
            vec3 pixel_sample = pixel00_loc + ((i+offset.x())*du) + ((j+offset.y())*dv);
            point3 ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample(); 

            vec3 ray_direction = pixel_sample - ray_origin;
            return ray(ray_origin, ray_direction);
        }

        vec3 sample_square() const {
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        point3 defocus_disk_sample() const {
            auto p = random_in_unit_disk();
            return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
        }


};

#endif