//Christoph Gründer, 01.07.2024
//Compile using: gcc -o echo_server echo_server.c -lws2_32

#include <stdio.h>
#include <winsock2.h>
#include <string.h>

#define PORT 8888
#define MAX_BUFFER_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET server_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_len = sizeof(client_addr);
    char buffer[MAX_BUFFER_SIZE];
    int number = 0;
    
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup failed. Error Code : %d", WSAGetLastError());
        return 1;
    }
    
    // Create a UDP socket
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) {
        printf("Failed to create socket. Error Code : %d", WSAGetLastError());
        return 1;
    }
    
    // Prepare the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    // Bind the socket to the server address
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed with error code : %d", WSAGetLastError());
        return 1;
    }
    
    printf("Echo server started on port %d\n", PORT);
    
    while (1) {
        // Receive data from the client
        int recv_len = recvfrom(server_socket, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_addr_len);
        if (recv_len == SOCKET_ERROR) {
            printf("recvfrom failed with error code : %d", WSAGetLastError());
            return 1;
        }
        
        // Print client's IP and port
        //printf("Received packet from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        // Extract the number from the message
            int received_number = atoi(buffer + 6);
        buffer[recv_len] = '\0';
        //printf("Received from server: %s\n", buffer);
            // Prepare the response
        //printf(".");
        
        // Check if the received message is of the form "hello <number>"
        if (strncmp(buffer, "hello ", 6) == 0) {
            
            
            // Echo back to the client
            if (sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, client_addr_len) == SOCKET_ERROR) {
                printf("sendto failed with error code : %d", WSAGetLastError());
                return 1;
            }
        }
    }
    
    closesocket(server_socket);
    WSACleanup();
    
    return 0;
}
