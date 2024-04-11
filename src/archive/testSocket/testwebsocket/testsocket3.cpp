#include <iostream>
#include <winsock2.h>

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

   

    int i = 0;
    while (true) {
        i++;
        // Dynamically allocate memory to store received data
        char* buffer = new char[1024];  // Adjust the size based on your message size
        int bytesRead = recv(serverSocket, buffer, 1024, 0);

        if (bytesRead > 0) {
            std::cout << "Received data from client [" << i << "]: " << buffer << std::endl;
        }

        delete[] buffer;  // Free the allocated memory
    }

    // Continue with the rest of your server code

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
