/**
 * @file socket.cpp
 * @brief Implementation file for the socket management functions.
 * 
 * This file contains the implementation of functions for initializing sockets,
 * sending and receiving data over sockets, and handling socket connections.
 * 
 * @author Christoph Gruender
 * @date 2024-05-17
 */

#include "socket.h"
#include <iostream>
#include <vector>
#include <string>

/**
 * @brief Constructor for SocketVerwaltung class.
 * 
 * Initializes the socket connection to the server.
 */
SocketVerwaltung::SocketVerwaltung() {
    // Initialize socket connection to the server
    initSocket("127.0.0.1", 12345);
}

/**
 * @brief Initializes a socket connection to a given IP address and port.
 * 
 * @param ip The IP address to connect to.
 * @param port The port to connect to.
 * @return int Status of the socket initialization (0 if successful, non-zero otherwise).
 */
int SocketVerwaltung::initSocket(std::string ip, int port) {
    // Create a client socket
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock\n";
        return 1;
    }
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error creating client socket\n";
        WSACleanup();
        return 1;
    }
    serverHint.sin_family = AF_INET;
    serverHint.sin_port = htons(port);
    serverHint.sin_addr.s_addr = inet_addr(ip.c_str());
    
    // Connect to the server
    int connResult = connect(clientSocket, reinterpret_cast<sockaddr*>(&serverHint), sizeof(serverHint));
    if (connResult == SOCKET_ERROR) {
        std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 2;
    }

    if(DEBUGLEVEL) std::cout << "Connected to Datasource!" << std::endl;

    // Send data to the server
    std::string sendData = "Hello, Server!";
    int sendResult = send(clientSocket, sendData.c_str(), sendData.size() + 1, 0);
    if (sendResult == SOCKET_ERROR) {
        std::cerr << "Send failed, Err #" << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 3;
    }
    clientSockets.push_back(clientSocket);
    std::cout << "connected to client supp with no err:" << WSAGetLastError();
    return 0;
}

/**
 * @brief Reads data from the socket.
 * 
 * @param s Pointer to the string to store the received data.
 * @return int Number of bytes read.
 */
int SocketVerwaltung::read(std::string* s) {
    for (SOCKET clientSocket : clientSockets) {
        // Dynamically allocate memory to store received data
        int bytesRead = recv(clientSocket, buffer, BUFSIZE, 0);

        if (bytesRead > 0) {
            if(DEBUGLEVEL) std::cout << "Received data from client:" << buffer << std::endl; else std::cout <<".";
            number_datepoints++;
            s->assign(buffer);
            if(DEBUGLEVEL) std::cout << ".";
            if (bytesRead > BUFSIZE) {
                printf("\n huge frame received, throwing away\n");
                s->assign("");
                memset(buffer, 0, BUFSIZE);
            } else {
                memset(buffer, 0, BUFSIZE);
                return bytesRead;
            }
        } else if (bytesRead == 0) {
            printf("no data rec");
        } else {
            std::cerr << "Read failed, Err #" << WSAGetLastError() << std::endl;
            if (WSAGetLastError() != 10014) {
                clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), clientSocket), clientSockets.end());
            }
        }
    }

    // On connection lost
    if (clientSockets.size() == 0) {
        std::cout << "Connection lost" << std::endl;
        int status = initSocket("127.0.0.1", 12345);
        if (status == 1) {
            std::cout << "Fatal Error, Server Connection lost. shutting down" << std::endl;
            return -1;
        } else if (status == 0) {
            std::cout << "Reconnection success." << std::endl;
            return read(s);
        } else {
            std::cout << "Reconnection unsuccessful." << std::endl;
            return 0;
        }
    }

    return 0;

    return 0;
}
