//
// Created by chris on 19.10.2023.
//
#include "calc.h"
#include <cassert>


calc::point calc::getPosFromAngles(std::vector<calc::SensorValue>sensorData, calc::point lastPoint){
    std::vector<calc::line>lines;
    //std::cout<<"maxindex: "<<maxindex;
    for (calc::SensorValue value :sensorData)
    {
        double angle=value.theta+value.val;
        double angle_rad=angle/180*M_PI;
        double xCord=value.xpos;
        std::cout<<" theta:"<<value.theta<<" sensor_out: "<<value.val<<" [angle: "<<sinf(angle_rad)<<"/"<<angle<<" pos: "<<xCord <<"] ";
        std::cout<<"["<<cosf(angle_rad)<<"|"<<sinf(angle_rad)<<"]\n";
        line l=line({xCord,0},{cosf(angle_rad),sinf(angle_rad)});
        
        lines.push_back(l);
    }
    if(std::isnan(lastPoint.getx())){
        lastPoint.setx(0.0);
    }
    if(std::isnan(lastPoint.gety())){
        lastPoint.sety(0.0);
    }

    //todo: implement
    point p=calc::gradientDescent(lastPoint,lines);

    std::cout<<"calculated pos ("<<p.getx()<<"|"<<p.gety()<<")"<<std::endl<<std::endl<<std::endl;
    return p;

}
bool calc::almostEqual(double a, double b, double epsilon) {
    return std::fabs(a - b) < epsilon;
}

std::vector<double> calc::line::getPointOnLine(){
    std::vector<double>result(this->start.size());;
    for (int i = 0; i < this->start.size(); i++)
    {
        result[i]=this->start[i]+this->direction[i];
    }
    return result;
    }

double calc::dotProduct(const std::vector<double>& v1, const std::vector<double>& v2) {
    double sum = 0.0;
    for (int i = 0; i < v1.size(); ++i) {
        sum += v1[i] * v2[i];
    }
    return sum;
}

std::vector<double> calc::vectorSubtract(const std::vector<double>& v1, const std::vector<double>& v2) {
    std::vector<double> result(v1.size());
    for (int i = 0; i < v1.size(); ++i) {
        result[i] = v1[i] - v2[i];
    }
    return result;
}

std::vector<double> calc::scalarMultiply(const std::vector<double>& v, double scalar) {
    std::vector<double> result(v.size());
    for (int i = 0; i < v.size(); ++i) {
        result[i] = v[i] * scalar;
    }
    return result;
}

double calc::vectorNorm(const std::vector<double>& v) {
    return std::sqrt(calc::dotProduct(v, v));
}

double calc::distance(point pnt,line l) {
    std::vector<double> A=l.start;
    std::vector<double> B=l.getPointOnLine();
    std::vector<double> P=pnt.position;

    std::vector<double> AP = vectorSubtract(P, A);
    std::vector<double> AB = vectorSubtract(B, A);

    double t = dotProduct(AP, AB) / dotProduct(AB, AB);
    std::vector<double> projection = scalarMultiply(AB, t);
    std::vector<double> d = vectorSubtract(AP, projection);
    return vectorNorm(d);
}


double calc::meanDistance(point p, std::vector<line> lines){
    double dist=0.0;
    for (line l:lines){
        dist+=calc::distance(p,l);
    }
    return dist/static_cast<double>(lines.size());
}
double calc::sqMeanDistance(point p, std::vector<line> lines){
    double dist=0.0;
    for (line l:lines){
        dist+=pow(calc::distance(p,l),2.0);
    }
    return dist/static_cast<double>(lines.size());
}

calc::line::line(std::vector<double> start, std::vector<double> direction){//ay+bx+c=0
    this->direction=direction;
    this->start=start;

}

calc::point::point(std::vector<double> f, double Unsicherheit){
    position=f;
    this->Unsicherheit=Unsicherheit;
}
double calc::point::getx(){
    return this->position[0];
}
void calc::point::setx(double x){
    this->position[0]=x;
}
double calc::point::gety(){
    return this->position[1];
}
void calc::point::sety(double y){
    this->position[1]=y;
}




unsigned char calc::doubleToUnsignedCharInRange(double value) {
    // Ensure value is within the range [0, 255]
    if (value < 0.0) {
        return 0;
    } else if (value > 255.0) {
        return 255;
    } else {
        return static_cast<unsigned char>(value);
    }
}

//Gradient descent: Q: Is dist to Lines convex?



calc::point calc::gradientDescent(calc::point startingPoint, std::vector<calc::line> lines,double gamma, int steps, double delta){
    if(steps==0||std::isnan(startingPoint.getx())||std::isnan(startingPoint.gety()))return startingPoint;
    double dist=sqMeanDistance(startingPoint,lines);
    double deltaDistX=dist-sqMeanDistance(point({startingPoint.getx()+delta,startingPoint.gety()}),lines);
    double incX=deltaDistX/delta;
    double deltaDistY=dist-sqMeanDistance(point({startingPoint.getx(),startingPoint.gety()+delta}),lines);
    double incY=deltaDistY/delta;
    startingPoint.setx(startingPoint.getx()+gamma*incX);
    startingPoint.sety(startingPoint.gety()+gamma*incY);
    return gradientDescent(startingPoint,lines,gamma,steps-1);
}