//
// Created by chris on 19.10.2023.
//

#ifndef STUDIENARBEIT_ANZEIGE_H
#define STUDIENARBEIT_ANZEIGE_H
#include "iostream"
#include "calc.h"
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <chrono>
class display {
public:
    /**Visualisierung der Position eines Punktes*/
    void projectPos(calc::point p);
    int setup();
    ~display();
    display();
private:
    SOCKET ConnectSocket;
    WSADATA wsaData;
    int result;
    struct sockaddr_in serverAddr;
    std::chrono::steady_clock::time_point lastSetupTime;
    const std::chrono::seconds setupInterval{10};
};


#endif //STUDIENARBEIT_ANZEIGE_H
