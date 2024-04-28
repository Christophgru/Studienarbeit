//
// Created by chris on 19.10.2023.
//

#include "display.h"
#include <cstring> // for std::strlen
#include <nlohmann/json.hpp>
using json = nlohmann::json;
bool reconnectNeeded = false;

void display::projectPos(std::vector<calc::SensorValue> angles, calc::point p) {
    try {
        json j;
        j["point"]["position"] = p.position;
        j["point"]["Unsicherheit"] = p.Unsicherheit;

        for (size_t i = 0; i < angles.size(); ++i) {
            json sensor;
            sensor["theta"] = angles[i].theta;
            sensor["val"] = angles[i].val;
            sensor["xpos"] = angles[i].xpos;
            j["sensor_values"].push_back(sensor);
        }

        // Convert JSON to string
        std::string jsonStr = j.dump();
        const char* message=jsonStr.c_str();

        // Output JSON string
        std::cout << jsonStr << std::endl;

        int result = send(ConnectSocket, message, strlen(message), 0);
        if (result == SOCKET_ERROR) {
          
            std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
            auto duration=setupInterval - (currentTime - lastSetupTime);
            //call only every 10 seconds, if called before, skip
            if(duration<=std::chrono::seconds{0}){
              std::cerr << "Send to Display failed: " << WSAGetLastError() <<" Bytes sent, try reconnect now"<<std::endl;
                setup();
                lastSetupTime = currentTime;
                return;
            }
            auto seconds =std::chrono::duration_cast<std::chrono::seconds>(duration);//get seconds
            auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000;
            //std::cerr << "Send to Display failed: " << WSAGetLastError() <<" Bytes sent, try reconnect in " <<seconds.count()<<"."<<milliseconds.count()<<"s"<< std::endl;//print seconds.milliseconds
            return;
        }
        std::cout << "sent " << result << " Bytes";
    } catch(const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << '\n';
        closesocket(ConnectSocket);
        WSACleanup();
    }
}



int display::setup() {
    // Initialize Winsock
    auto start = std::chrono::steady_clock::now();
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Initialization time: " << duration.count() << "ms" << std::endl;

    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }

    // Create a socket
    start = std::chrono::steady_clock::now();
    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Socket creation time: " << duration.count() << "ms" << std::endl;

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
    timeout.tv_usec = 20 ; // 20 milliseconds
    setsockopt(ConnectSocket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
    // Connect to the server
    start = std::chrono::steady_clock::now();
    result = connect(ConnectSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Connection time: " << duration.count() << "ms" << std::endl;

    if (result == SOCKET_ERROR) {
        std::cerr << "Error connecting to Display: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
    
    return 0;
}



display::~display(){
  // Clean up
    closesocket(ConnectSocket);
    WSACleanup();
}

display::display(){
  setup();
}