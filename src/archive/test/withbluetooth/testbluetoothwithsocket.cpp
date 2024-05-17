#include <iostream>
#include <winsock2.h>
#include <chrono>
#include <iomanip>

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock\n";
        return 1;
    }

    // Create a socket, bind, listen, and accept a connection (as in your previous code)
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(12345);  // Port number

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Error binding socket\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Error listening on socket\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server listening on port 12345...\n";

    sockaddr_in clientAddress;
    int clientSize = sizeof(clientAddress);
    SOCKET clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddress), &clientSize);

    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error accepting client connection\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Get the current time
    auto start = std::chrono::high_resolution_clock::now();
    int number_datepoints=0;
    try
    {
        /* code */

        for(int i=0;i<100000;i++){
            
        
            
            // Dynamically allocate memory to store received data
            char* buffer = new char[1024];  // Adjust the size based on your message size
            int bytesRead = recv(clientSocket, buffer, 1024, 0);

            if (bytesRead > 0) {
                std::cout << "Received data from client["<<i<<"]:" << buffer << std::endl;
                number_datepoints++;
            }
            
            delete[] buffer;  // Free the allocated memory
        
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    // Perform some operations or code for which you want to measure the time

    // Get the time after the operations
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // Convert the duration to seconds with three digits after the comma
    double seconds = duration.count() / 1e6;

    // Print the time difference in seconds
  std::cout << "Time difference: " << std::fixed << std::setprecision(3) << seconds << " seconds, "<< number_datepoints 
            <<" Datapoints resulting in "<< number_datepoints/seconds <<" fps" << std::endl;


    // Continue with the rest of your server code

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
