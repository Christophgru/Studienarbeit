
#include "calc.h"
#include <iostream>
#include <fstream>
struct Pixel {
    unsigned char b, g, r;
};

Pixel getColorFromValue(float value, int maxValue=100);
void paintpicture( std::vector<calc::line> lines, int width, int height, float zoomfactor=10, int maxDistColorGrad=10);