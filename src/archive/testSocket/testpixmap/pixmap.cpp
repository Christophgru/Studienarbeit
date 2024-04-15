#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "../../../Backend/calc.h"
const int WIDTH = 200;
const int HEIGHT = 200;

struct Pixel {
    unsigned char b, g, r;
};

using namespace calc;

Pixel interpolateColor(Pixel startColor, Pixel endColor, float t) {
    Pixel result;
    result.r = startColor.r + static_cast<unsigned char>((endColor.r - startColor.r) * t);
    result.g = startColor.g + static_cast<unsigned char>((endColor.g - startColor.g) * t);
    result.b = startColor.b + static_cast<unsigned char>((endColor.b - startColor.b) * t);
    return result;
}
 // Define color gradient ranges
        Pixel white = {255, 255, 255};
        Pixel yellow = {255, 255, 0};
        Pixel green = {0, 255, 0};
        Pixel red = {255, 0, 0};
        Pixel black = {0, 0, 0};

Pixel getColorFromValue(float value){
    Pixel color;
    if (value <= 1000) {
                    color = interpolateColor(white, yellow, value / 1000.0f);
                } else if (value <= 2000) {
                    color = interpolateColor(yellow, green, (value - 1000) / 1000.0f);
                } else if (value <= 3000) {
                    color = interpolateColor(green, red, (value - 2000) / 1000.0f);
                } else if(value <= 5000){
                    color = interpolateColor(red, black, (value - 3000) / 2000.0f);
                }else{ 
                    color=black;
                }
    return color;
}


int main() {
    try {
        // Create an array to hold pixel values
        std::ofstream outFile("../out/output_image.ppm", std::ios::binary);

        std::vector<line>vec;
        line l1=line(10,10);
        line l2=line(20,-1);
        line l3=line(0,1);
        vec.push_back(l1);
        vec.push_back(l2);
        vec.push_back(l3);


        Pixel* image = new Pixel[WIDTH * HEIGHT];
        //todo find max by first checking for the corners, then map colors on range [0,max]
        //todo fix ugly interpolation

        // Set the value of each pixel
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                float coordx=static_cast<float>(x) / 10.0f;
                float coordy=static_cast<float>(y) / 10.0f;
                point p=point(coordx,coordy);
                float value=distance(p,vec)*800;
                unsigned char c=floatToUnsignedCharInRange(value);
                //if(x%10==0&&y%10==0)std::cout<<"x:"<<x<<"; y:"<<y<<"; coordx:"<<coordx<<"; coordy:"<<coordy<<"; val"<<value<<"; val[255]:"<<c<<std::endl;
                // Calculate the y-coordinate in the inverted image
                
        int invertedY = HEIGHT - y - 1;
        // Set RGB values for each pixel (here we set it to green)
        //image[invertedY * WIDTH + x] = {c, c, c}; // (B, G, R)

                // Determine the color based on the value
                Pixel color;
                color=getColorFromValue(value);
                
                
                // Set RGB values for each pixel
                image[invertedY * WIDTH + x] = color;
             }
        }

        // Write the image to storage
        outFile << "P6\n" << WIDTH << " " << HEIGHT << "\n255\n";
        outFile.write(reinterpret_cast<const char*>(image), WIDTH * HEIGHT * sizeof(Pixel));

        outFile.close();

        // Free dynamically allocated memory
        delete[] image;
    } catch(const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}