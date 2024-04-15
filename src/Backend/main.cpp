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
     
    //todo: Übergabeparameter auswerten
    std::vector<float> positions;
    display* d=new display();
    //todo: serial in liste oder vektor einlesen, um anzahl dynamisch zu machen
    SocketVerwaltung s = SocketVerwaltung();
    while (true){
        std::string string="";
        int red=s.read(string);
        if(0<red){
            printf(string.c_str());
            std::vector<float> angles=getAnglesFromString(string);
            //Kalmann filter
            point p=calc::getPosFromAngles(angles,positions);
            d->projectPos(p);
            int x=0;
        }else if (red!=0)
        {
            break;
        }
        
    }
}
    