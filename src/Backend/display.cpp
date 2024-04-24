//
// Created by chris on 19.10.2023.
//
#include "iostream"
#include "display.h"
void display::projectPos(calc::point p) {
    //erstmal nur cmd ausgabe, später gui?
    try
    {
       std::cout<<"Xpos="<< p.getx()<<" \t\tYpos="<<p.gety()<<" Abweichung="<<p.abweichung<< std::endl;
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
    }
    
     
}