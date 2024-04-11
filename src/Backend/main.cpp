//
// Created by chris on 19.10.2023.
//

#include "socket.h"
#include "iostream"
#include "calc.h"
#include "display.h"
int main(int argc, char** argv){
    //todo: Übergabeparameter auswerten
    int distance;
    calc* c=new calc();
    display* d=new display();
    //todo: serial in liste oder vektor einlesen, um anzahl dynamisch zu machen
    SocketVerwaltung s = SocketVerwaltung();
    std::string string;
    while (true){
        int red=s.read(string);
        if(0<red){
            printf(string.c_str());
            float angle1;
            float angle2;
            //Kalmann filter
            point p=c->getPosFromAngles(angle1,angle2,distance);
            d->projectPos(p);
            int x=0;
        }else if (red!=0)
        {
            break;
        }
        
    }
}
    