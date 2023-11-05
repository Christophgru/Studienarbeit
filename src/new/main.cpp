//
// Created by chris on 19.10.2023.
//

#include "serial.h"
#include "iostream"
#include "calc.h"
#include "display.h"
int main(int argc, char** argv){
    //todo: Übergabeparameter auswerten
    int port1=11;
    int port2=12;
    float abstand=5;
    calc* c=new calc();
    display* d=new display();
    //todo: serial in liste oder vektor einlesen, um anzahl dynamisch zu machen
    serial s1=serial(port1);
    serial s2=serial(port2);
    float angle1=s1.getAngle();
    float angle2=s2.getAngle();
    point p=c->getPosFromAngles(angle1,angle2,abstand);
    d->projectPos(p);
    int x=0;
    while (x<1000000000)x++;
}