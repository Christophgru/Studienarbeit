//
// Created by chris on 19.10.2023.
//
#include "calc.h"


calc::point calc::getPosFromAngles(std::vector<float>angles,std::vector<float>xCords){
    std::vector<calc::line>lines;
    int maxindex=(angles.size() < xCords.size()) ? angles.size() : xCords.size();
    std::cout<<"maxindex: "<<maxindex;
    for (int i = 0; i < maxindex; i++)
    {
        float angle=angles.at(i);
        float angle_rad=angle/180*M_PI;
        float xCord=xCords.at(i);
        std::cout<<"[angle: "<<sinf(angle_rad)<<"/"<<angle<<" pos: "<<xCord <<"]\t ";
        std::cout<<"["<<sinf(angle_rad)<<"|"<<-cosf(angle_rad)<<"]\t";
        line l=line({xCord,0},{sinf(angle_rad),-cosf(angle_rad)});
        
        lines.push_back(l);
    }
    //todo: implement
    point p=calc::gradientDescent(point({0.0,0.0}),lines);

   // std::cout<<"calculated pos ("<<p.x<<"|"<<p.y<<")"<<" from az1:"<<resultAzimuth1<<" az2:"<<resultAzimuth2<<" abstand: "<<abstand<<std::endl;
    return p;

}
bool calc::almostEqual(double a, double b, double epsilon) {
    return std::fabs(a - b) < epsilon;
}

float calc::distance(point pnt,line l){
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




float calc::meanDistance(point p, std::vector<line> lines){
    float dist=0.0f;
    for (line l:lines){
        dist+=calc::distance(p,l);
    }
    return dist/static_cast<float>(lines.size());
}
float calc::sqMeanDistance(point p, std::vector<line> lines){
    float dist=0.0f;
    for (line l:lines){
        dist+=pow(calc::distance(p,l),2.0f);
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
float calc::point::getx(){
    return this->position[0];
}
void calc::point::setx(float x){
    this->position[0]=x;
}
float calc::point::gety(){
    return this->position[1];
}
void calc::point::sety(float y){
    this->position[1]=y;
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



calc::point calc::gradientDescent(calc::point startingpoint, std::vector<calc::line> lines,float gamma, int steps, float delta){
    if(steps==0||startingpoint.getx()==NAN||startingpoint.gety()==NAN)return startingpoint;
    float dist=sqMeanDistance(startingpoint,lines);
    //dx
    float deltaDistX=dist-sqMeanDistance(point({startingpoint.getx()+delta,startingpoint.gety()}),lines);
    float incX=deltaDistX/delta;
    //dy
     float deltaDistY=dist-sqMeanDistance(point({startingpoint.getx(),startingpoint.gety()+delta}),lines);
    float incY=deltaDistY/delta;
    startingpoint.setx(startingpoint.getx()+gamma*incX);
    startingpoint.sety(startingpoint.gety()+gamma*incY);
    return gradientDescent(startingpoint,lines,gamma,steps-1);
}