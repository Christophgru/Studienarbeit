//
// Created by chris on 19.10.2023.
//

#ifndef STUDIENARBEIT_CALC_H
#define STUDIENARBEIT_CALC_H

/** Datenklasse für Ergebnis der Punktberechnungs methode*/
struct point{
    float x;
    float y;
    float abweichung;
};

/**Klasse für algorithmik wie winkelberechnung oder mittelwertbildung*/
class calc {

public :
    /**getPos
     * @param resultAzimuth1: Messergebnis des ersten sensors
     * @param resultAzimuth2: Messergebnis des zweiten sensors
     * @param abstand: Abstand zwischen den beiden sensoren
     * @param theta1: abweichung des Azimuths von einer parallelen ausrichtung zum anderen Sensor
     * @param theta2: abweichung des Azimuths von einer parallelen ausrichtung zum anderen Sensor*/
    point getPosFromAngles(float resultAzimuth1, float resultAzimuth2, float abstand, float theta1=0, float theta2=0);
    //todo overload getPosFromAngles for more datasources
};


#endif //STUDIENARBEIT_CALC_H
