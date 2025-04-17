#include <iostream>
#include "utils.h"

int main(){

    int img_width = 2048;
    int img_height = 2048;


    std::cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";
    for(int i = 0; i < img_height; i++){
        std::clog << "\rScanlines remaining: " << (img_height - i) << ' ' << std::flush; // 
        for(int j = 0; j < img_width; j++){
            // increase redness with height & increase blueness with width
            color pixel_color = color(double(i) / double(img_height - 1), 0.0, double(j) / double(img_width - 1));
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone :)                     \n";


    return 0;
}