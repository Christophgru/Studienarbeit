/**
 * @file socket.h
 * @brief Header file for the socket management functions.
 * 
 * This file contains the declarations of functions and classes for initializing sockets,
 * sending and receiving data over sockets, and handling socket connections.
 * 
 * @author Christoph Gruender
 * @date 2024-05-17
 */

#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <winsock2.h>
#include <chrono>
#include <iomanip>
#include <vector>
#include <algorithm>

#define DEBUGLEVEL 1
#define BUFSIZE 1048

/**
 * @class SocketVerwaltung
 * @brief Class for managing socket connections.
 */
class SocketVerwaltung {
    int number_datepoints = 0; ///< Number of data points received
    std::vector<SOCKET> clientSockets; ///< Vector of client sockets
    sockaddr_in serverHint; ///< Server address hint
    char* buffer = new char[BUFSIZE]; ///< Buffer for receiving data
    int initSocket(std::string ip, int port); ///< Initializes a socket connection
    WSADATA wsaData; ///< Winsock data

public:
    /**
     * @brief Constructor for SocketVerwaltung class.
     * 
     * Initializes the socket connection to the server.
     */
    SocketVerwaltung();

    /**
     * @brief Reads data from the socket.
     * 
     * @param s Pointer to the string to store the received data.
     * @return int Number of bytes read.
     */
    int read(std::string* s);
};

#endif // SOCKET_H
