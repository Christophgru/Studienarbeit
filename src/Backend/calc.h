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
        std::vector<float>position;
        float abweichung;
        point(std::vector<float> f, float abweichung=0);
        void setx(float x);
        float getx();
        void sety(float y);
        float gety();
    };
    struct line{
        std::vector<float>start;
        std::vector<float>direction;
        line(std::vector<float> start, std::vector<float> direction);
        
    };


    /**getPos
     * @param resultAzimuth1: Messergebnis des ersten sensors
     * @param resultAzimuth2: Messergebnis des zweiten sensors
     * @param abstand: Abstand zwischen den beiden sensoren
     * @param theta1: abweichung des Azimuths von einer parallelen ausrichtung zum anderen Sensor
     * @param theta2: abweichung des Azimuths von einer parallelen ausrichtung zum anderen Sensor*/
    point getPosFromAngles(std::vector<float>angles,std::vector<float>xCords);
    //todo overload getPosFromAngles for more datasources
    float distance(point,line);
    float meanDistance(point p, std::vector<line> lines);
    float sqMeanDistance(point p,std::vector<line> lines);
    unsigned char floatToUnsignedCharInRange(float value);
    bool almostEqual(double a, double b, double epsilon = 1e-10);
    point gradientDescent(point startingpoint, std::vector<line> lines, float gamma=0.5f,int steps=20,float delta=0.0001f);
};


#endif //STUDIENARBEIT_CALC_H