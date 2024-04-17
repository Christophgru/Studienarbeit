#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "../../../Backend/calc.h"
#include "painthelper.h"
const int WIDTH = 200;
const int HEIGHT = 200;



using namespace calc;

int main() {
    try {
        // Create an array to hold pixel values
        std::ofstream outFile("../out/output_image.ppm", std::ios::binary);

        std::vector<line>vec;
        line l1=line({2,0},{1,1});
        line l2=line({10,0},{0,1});
        line l3=line({18,0},{-1,1});
        line l4=line({18,0},{0,1});
        vec.push_back(l1);
        vec.push_back(l2);
        vec.push_back(l3);
        vec.push_back(l4);


        Pixel* image = new Pixel[WIDTH * HEIGHT];
        //todo find max by first checking for the corners, then map colors on range [0,max]
        //todo fix ugly interpolation
        float maxVal=0;
        float zoomfactor=10;
        Pixel lastColor={0,0,0};
        // Set the value of each pixel
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                float coordx=static_cast<float>(x)/zoomfactor;
                float coordy=static_cast<float>(y)/zoomfactor;
                point p=point({coordx,coordy});
                float value=distance(p,vec);
                 //if(x%10==0&&y%10==0) std::cout<<"x:"<<x<<"; y:"<<y<<";\t coordx:"<<coordx<<"; coordy:"<<coordy<<";\tc val: "<<value<<std::endl;
                if(value>maxVal)maxVal=value;
                // Determine the color based on the value
                
                Pixel color;
                color=getColorFromValue(value,10);
                
                
                
                if(abs(color.g-lastColor.g)>200||
                abs(color.r-lastColor.r)>200||
                abs(color.b-lastColor.b)>200 ){
                    std::cout<<"weird jump ["<<x<<"|"<<y<<"]: {"<<static_cast<int> (color.r)<<"|"<<static_cast<int> (color.g)<<"|"
                    <<static_cast<int> (color.b)<<"}\n";
                }
                

                lastColor=color;
                int invertedY = HEIGHT - y - 1;
                // Set RGB values for each pixel
                image[invertedY * WIDTH + x] = color;
             }
        }
        std::cout<<"maxVal="<<maxVal<<std::endl;
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