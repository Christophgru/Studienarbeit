//
// Created by chris on 19.10.2023.
//
#include "calc.h"


calc::point calc::getPosFromAngles(std::vector<float>angles,std::vector<float>xCords){
    //todo: implement
    point p=point({0,0,0});
   // std::cout<<"calculated pos ("<<p.x<<"|"<<p.y<<")"<<" from az1:"<<resultAzimuth1<<" az2:"<<resultAzimuth2<<" abstand: "<<abstand<<std::endl;
    return p;

}
bool calc::almostEqual(double a, double b, double epsilon) {
    return std::fabs(a - b) < epsilon;
}

float calc::getDistance(point pnt,line l){
    std::vector<float> r=l.direction;
    std::vector<float> ro={-r.at(1),r.at(0)};
    std::vector<float> p=pnt.position;
    std::vector<float> u=l.start;
    float up=r.at(1)*(p.at(0)-u.at(0))-r.at(0)*(p.at(1)-u.at(1));
    float down=ro.at(0)*r.at(1)-ro.at(1)*r.at(0);
    float lambda2=fabs(up/down);

    float dist=lambda2*sqrt(ro.at(0)*ro.at(0)+ro.at(1)*ro.at(1));


    return dist;
}




float calc::distance(point p, std::vector<line> lines){
    float dist=0.0f;
    for (line l:lines){
        dist+=calc::getDistance(p,l);
    }
    return dist/static_cast<float>(lines.size());
}

calc::line::line(std::vector<float> start, std::vector<float> direction){//ay+bx+c=0
    this->direction=direction;
    this->start=start;

}

calc::point::point(std::vector<float> f, float abweichung){
    position=f;
    this->abweichung=abweichung;
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