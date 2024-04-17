
#include "painthelper.h"

Pixel white = {255, 255, 255};
Pixel yellow = {255, 255, 0};
Pixel green = {0, 255, 0};
Pixel red = {255, 0, 0};
Pixel black = {0, 0, 0};

Pixel interpolateColor(Pixel startColor, Pixel endColor, float t) {
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



Pixel getColorFromValue(float value, int maxValue){
    float mapped=map(value, 0,maxValue,0,5);
    Pixel color;
    
    if (value <= 1.0f) {
        color = interpolateColor(white, yellow, value );
    } else if (value <= 2.0f) {
        color = interpolateColor(yellow, green, value  -1.0f);
    } else if (value <= 3.0f) {
        color = interpolateColor(green, red, value  -2.0f);
    } else if(value <= 5.0f){
        color = interpolateColor(red, black, (value  -3.0f)/2.0f);
    }else{ 
        color=black;
    }
    return color;
}