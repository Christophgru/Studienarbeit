//
// Created by chris on 19.10.2023.
//

#include "serial.h"
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
    s1.initialize();
    s2.initialize();
    point p=c->getPosFromAngles(s1.getAngle(),s2.getAngle(),abstand);
    d->projectPos(p);
}