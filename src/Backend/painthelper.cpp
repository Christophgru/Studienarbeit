
#include "painthelper.h"


Pixel white = {255, 255, 255};
Pixel yellow = {255, 255, 0};
Pixel green = {0, 255, 0};
Pixel red = {255, 0, 0};
Pixel black = {0, 0, 0};

Pixel interpolateColor(Pixel startColor, Pixel endColor, double t) {
    Pixel result;
    result.r = startColor.r + static_cast<unsigned char>((endColor.r - startColor.r) * t);
    result.g = startColor.g + static_cast<unsigned char>((endColor.g - startColor.g) * t);
    result.b = startColor.b + static_cast<unsigned char>((endColor.b - startColor.b) * t);
    return result;
}
double map(double x, double in_min, double in_max, double out_min, double out_max) {
    // Check for invalid input
    if (x < in_min) x = in_min;
    if (x > in_max) x = in_max;

    // Calculate the proportion of x in the input range
    double proportion = (x - in_min) / (in_max - in_min);

    // Scale the proportion to the output range and return
    return proportion * (out_max - out_min) + out_min;
}



Pixel getColorFromValue(double value, int maxValue){
    double mapped=map(value, 0,maxValue,0,5);
    Pixel color;
    //std::cout<<"val: "<<value<<" mapped: "<<mapped<<std::endl;
    if (mapped <= 1.0f) {
        color = interpolateColor(white, yellow, mapped );
    } else if (mapped <= 2.0f) {
        color = interpolateColor(yellow, green, mapped  -1.0f);
    } else if (mapped <= 3.0f) {
        color = interpolateColor(green, red, mapped  -2.0f);
    } else if(mapped <= 5.0f){
        color = interpolateColor(red, black, (mapped  -3.0f)/2.0f);
    }else{ 
        color=black;
    }
    return color;
}

using namespace calc;

void paintpicture(std::vector<line> lines, int WIDTH, int HEIGHT, double zoomfactor, int maxDistColorGrad){


    try {
        // Create an array to hold pixel values
        std::ofstream outFile("D:/Programmieren/Studienarbeit/out/output_image.ppm", std::ios::binary);


        Pixel* image = new Pixel[WIDTH * HEIGHT];
        //todo find max by first checking for the corners, then map colors on range [0,max]
        //todo fix ugly interpolation
        double maxVal=0;
        Pixel lastColor={0,0,0};
        // Set the value of each pixel
        for (int x = 0; x < WIDTH; ++x) {
            for (int y = 0; y < HEIGHT; ++y) {
                double coordx=static_cast<double>(x)/zoomfactor;
                double coordy=static_cast<double>(y)/zoomfactor;
                point p=point({coordx,coordy});
                double value=sqMeanDistance(p,lines);
                if(value>maxVal)maxVal=value;
                // Determine the color based on the value
                Pixel color;
                color=getColorFromValue(value,maxDistColorGrad);
                lastColor=color;
                int invertedY = HEIGHT - y - 1;
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
    
        
        std::cout<<"maxVal="<<maxVal<<std::endl;

    } catch(const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
}