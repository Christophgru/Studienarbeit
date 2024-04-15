//
// Created by chris on 19.10.2023.
//
#include "calc.h"


calc::point calc::getPosFromAngles(std::vector<float>angles,std::vector<float>xCords){
    //todo: implement


    
    point p=point(0,0,0);
   // std::cout<<"calculated pos ("<<p.x<<"|"<<p.y<<")"<<" from az1:"<<resultAzimuth1<<" az2:"<<resultAzimuth2<<" abstand: "<<abstand<<std::endl;
    return p;

}

float calc::getDistance(point p,line l){
    float a=l.elevation;
    float b=-1;
    float c=-l.x*l.elevation;

    float dist=abs(a*p.x+b*p.y+c)/sqrt(a*a+b*b);
    return dist;
}




float calc::distance(point p, std::vector<line> lines){
    float dist=0;
    for (line l:lines){
        dist+=calc::getDistance(p,l);
    }
    return dist/lines.size();
}



unsigned char calc::floatToUnsignedCharInRange(float value) {
    // Ensure value is within the range [0, 255]
    if (value < 0.0f) {
        return 0;
    } else if (value > 255.0f) {
        return 255;
    } else {
        return static_cast<unsigned char>(value);
    }
}

//Gradient descent: Q: Is dist to Lines convex?