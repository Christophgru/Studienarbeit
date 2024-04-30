//
// Created by chris on 19.10.2023.
//

#include "socket.h"
#include "iostream"
#include "calc.h"
#include "display.h"
#include <vector>
#include "stringparser.h"


int main(int argc, char** argv){
     
    display* d=new display();
    //todo: serial in liste oder vektor einlesen, um anzahl dynamisch zu machen
    SocketVerwaltung s = SocketVerwaltung();
    calc::point lastValue=calc::point({0.0,0.0});
    while (true){
        std::string string="";
        int red=s.read(&string);
        if(0<red){
            std::vector<calc::SensorValue> angles=getangles(string);
            lastValue=calc::getPosFromAngles(angles,lastValue);
            //Kalmann filter
            d->projectPos(angles, lastValue);
            int x=0;
        }else if (red!=0)
        {
            printf("error");
            break;
        }
        
    }
}
    