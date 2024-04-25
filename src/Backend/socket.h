#include <iostream>
#include <winsock2.h>
#include <chrono>
#include <iomanip>
#include <vector>
#include <algorithm>
#define BUFSIZE 1048

class SocketVerwaltung{
    int number_datepoints=0;
    std::vector<SOCKET>clientSockets;
    sockaddr_in serverHint;
    char* buffer = new char[BUFSIZE];  // Adjust the size based on your message size
    int initSocket(std::string ip, int port);
public:
    SocketVerwaltung();
    int read(std::string* s);

};