/**
 * @file display.cpp
 * @brief Implementation file for the display library.
 * 
 * This file contains the implementation of functions to project positions and 
 * communicate with a display server using sockets.
 * 
 * @author Christoph Gruender
 * @date 2024-05-17
 */

#include "display.h"
using json = nlohmann::json;

bool reconnectNeeded = false;

/**
 * @brief Projects the position and sends it to the display server.
 * 
 * @param angles Vector of sensor values.
 * @param p Point containing position and uncertainty.
 */
void display::projectPos(std::vector<calc::SensorValue> angles, calc::point p) {
    try {
        json j;
        j["point"]["position"] = p.position;
        j["point"]["Uncertainty"] = p.uncertainty;
        std::cout<<"\n\n"<<p.getx()<< " | "<<p.gety()<<";\t"<<angles.size()<< " values: ";

        for (size_t i = 0; i < angles.size(); ++i) {
            json sensor;
            sensor["theta"] = angles[i].theta;
            sensor["val"] = angles[i].val;
            sensor["xpos"] = angles[i].xpos;
            j["sensor_values"].push_back(sensor);
            std::cout<<i<< " : val:"<<angles[i].val<<"theta: " <<angles[i].theta<<" xpos:"<< angles[i].xpos;
        }
        

        // Convert JSON to string
        std::string jsonStr = j.dump();
        const char* message = jsonStr.c_str();

        // Output JSON string
        if(DEBUGLEVEL) std::cout << jsonStr << std::endl;

        int result = send(ConnectSocket, message, strlen(message), 0);
        if (result == SOCKET_ERROR) {
            std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
            auto duration = setupInterval - (currentTime - lastSetupTime);
            // Call only every 10 seconds, if called before, skip
            if (duration <= std::chrono::seconds{0}) {
                std::cerr << "Send to Display failed: " << WSAGetLastError() << " Bytes sent, try reconnect now" << std::endl;
                setup();
                lastSetupTime = currentTime;
                return;
            }
            auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration); // Get seconds
            auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000;
            // std::cerr << "Send to Display failed: " << WSAGetLastError() <<" Bytes sent, try reconnect in " <<seconds.count()<<"."<<milliseconds.count()<<"s"<< std::endl; // Print seconds.milliseconds
            return;
        }
        if(DEBUGLEVEL) std::cout << "sent " << result << " Bytes";
    } catch(const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << '\n';
        closesocket(ConnectSocket);
        WSACleanup();
    }
}

/**
 * @brief Sets up the connection to the display server.
 * 
 * @return int Status of the setup operation (0 if successful, 1 otherwise).
 */
int display::setup() {
    // Initialize Winsock
    auto start = std::chrono::steady_clock::now();
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    if(DEBUGLEVEL) std::cout << "Initialization time: " << duration.count() << "ms" << std::endl;

    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }

    // Create a socket
    start = std::chrono::steady_clock::now();
    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    if(DEBUGLEVEL) std::cout << "Socket creation time: " << duration.count() << "ms" << std::endl;

    if (ConnectSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Resolve the server address and port
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12346);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    // Set the timeout for the connect function to 20 milliseconds
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 20; // 20 milliseconds
    setsockopt(ConnectSocket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));

    // Connect to the server
    start = std::chrono::steady_clock::now();
    result = connect(ConnectSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    if(DEBUGLEVEL) std::cout << "Connection time: " << duration.count() << "ms" << std::endl;

    if (result == SOCKET_ERROR) {
        std::cerr << "Error connecting to Display: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
    
    return 0;
}

/**
 * @brief Destructor for display.
 * 
 * Cleans up by closing the socket and cleaning up Winsock.
 */
display::~display() {
    closesocket(ConnectSocket);
    WSACleanup();
}

/**
 * @brief Constructor for display.
 * 
 * Sets up the connection to the display server.
 */
display::display() {
    setup();
}
