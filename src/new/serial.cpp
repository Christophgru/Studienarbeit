//
// Created by chris on 19.10.2023.
//
#include "iostream"
#include "serial.h"
serial::serial(int port) {
    this->port=port;

    std::cout<<"port"<< port<<" erstellt"<<std::endl;
    this->initialize();
}

int serial::initialize() {
    std::cout<<"port"<< port<<" initialisiert"<<std::endl;
    return 1;
}

float serial::getAngle() {
    std::cout<<"port:"<<port<<" Winkel empfangen"<<std::endl;
    return 0;
}