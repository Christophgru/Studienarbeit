//
// Created by chris on 19.10.2023.
//

#ifndef STUDIENARBEIT_CALC_H
#define STUDIENARBEIT_CALC_H
#include <vector>
#include "cmath"
#include "iostream"

/**Klasse für algorithmik wie winkelberechnung oder mittelwertbildung*/
namespace calc {

    
/** Datenklasse für Ergebnis der Punktberechnungs methode*/
    struct point{
        std::vector<double>position;
        double Unsicherheit;
        point(std::vector<double> f, double Unsicherheit=0);
        void setx(double x);
        double getx();
        void sety(double y);
        double gety();
    };
    struct line{
        std::vector<double>start;
        std::vector<double>direction;
        line(std::vector<double> start, std::vector<double> direction);
        std::vector<double>getPointOnLine();
    };


    struct SensorValue {
    double theta;
    double val;
    double xpos;
    };


    /**getPos
     * @param resultAzimuth1: Messergebnis des ersten sensors
     * @param resultAzimuth2: Messergebnis des zweiten sensors
     * @param abstand: Abstand zwischen den beiden sensoren
     * @param theta1: Unsicherheit des Azimuths von einer parallelen ausrichtung zum anderen Sensor
     * @param theta2: Unsicherheit des Azimuths von einer parallelen ausrichtung zum anderen Sensor*/
    point getPosFromAngles(std::vector<calc::SensorValue>sensorData, calc::point lastPoint);
    //todo overload getPosFromAngles for more datasources
    double distance(point,line);
    double meanDistance(point p, std::vector<line> lines);
    double sqMeanDistance(point p,std::vector<line> lines);
    unsigned char doubleToUnsignedCharInRange(double value);
    bool almostEqual(double a, double b, double epsilon = 1e-10);
    point gradientDescent(point startingpoint, std::vector<line> lines, double gamma=0.5,int steps=20,double delta=1e-10);

    double dotProduct(const std::vector<double>& v1, const std::vector<double>& v2);
    std::vector<double> vectorSubtract(const std::vector<double>& v1, const std::vector<double>& v2);
    std::vector<double> scalarMultiply(const std::vector<double>& v, double scalar);
    double vectorNorm(const std::vector<double>& v);

};


#endif //STUDIENARBEIT_CALC_H
