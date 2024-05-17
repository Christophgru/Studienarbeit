/**
 * @file display.h
 * @brief Header file for the display library.
 * 
 * This file contains the declarations of functions and classes used to 
 * project positions and communicate with a display server using sockets.
 * 
 * @author Christoph Gruender
 * @date 2024-05-17
 */

#ifndef STUDIENARBEIT_ANZEIGE_H
#define STUDIENARBEIT_ANZEIGE_H

#include "iostream"
#include "calc.h"
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <chrono>
#include <cstring> // for std::strlen
#include <nlohmann/json.hpp>

/**
 * @class display
 * @brief Class for projecting positions and communicating with a display server.
 */
class display {
public:
    /**
     * @brief Visualizes the position of a point.
     * @param angles Vector of sensor values.
     * @param p Point containing position and uncertainty.
     */
    void projectPos(std::vector<calc::SensorValue> angles, calc::point p);

    /**
     * @brief Sets up the connection to the display server.
     * @return int Status of the setup operation (0 if successful, 1 otherwise).
     */
    int setup();

    /**
     * @brief Destructor for display.
     * 
     * Cleans up by closing the socket and cleaning up Winsock.
     */
    ~display();

    /**
     * @brief Constructor for display.
     * 
     * Sets up the connection to the display server.
     */
    display();

private:
    SOCKET ConnectSocket; ///< Socket for connection
    WSADATA wsaData; ///< Winsock data
    int result; ///< Result of operations
    struct sockaddr_in serverAddr; ///< Server address
    std::chrono::steady_clock::time_point lastSetupTime; ///< Last setup time
    const std::chrono::seconds setupInterval{10}; ///< Setup interval
};

#endif // STUDIENARBEIT_ANZEIGE_H
