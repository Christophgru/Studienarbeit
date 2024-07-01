//Christoph Gründer, 01.07.2024
//Compile using: gcc -o echo_client echo_client.c -lws2_32

#include <stdio.h>
#include <winsock2.h>
#include <signal.h>
#include <time.h>

#define PORT 8888
#define MAX_BUFFER_SIZE 1024

// Global variables to track statistics
int message_count = 0;
clock_t start_time;

// Signal handler function for Ctrl+C
void ctrl_c_handler(int sig) {
    double elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    double fps = message_count / elapsed_time;
    
    printf("\nStatistics:\n");
    printf("Messages sent: %d\n", message_count);
    printf("Elapsed time: %.2f seconds\n", elapsed_time);
    printf("Average FPS: %.2f\n", fps);
    
    exit(0);
}

int main() {
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server_addr;
    int server_addr_len = sizeof(server_addr);
    char buffer[MAX_BUFFER_SIZE];
    
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup failed. Error Code : %d", WSAGetLastError());
        return 1;
    }
    
    // Create a UDP socket
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) {
        printf("Failed to create socket. Error Code : %d", WSAGetLastError());
        return 1;
    }
    
    // Prepare the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address
    server_addr.sin_port = htons(PORT);
    
    // Register signal handler for Ctrl+C
    signal(SIGINT, ctrl_c_handler);
    
    int count = 0;
    char recv_msg[MAX_BUFFER_SIZE];
    
    // Record the start time
    start_time = clock();
    
    while (1) {
        // Prepare the message to send
        sprintf(buffer, "hello %d", count);
        
        // Send the message to the server
        if (sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, server_addr_len) == SOCKET_ERROR) {
            printf("sendto failed with error code : %d", WSAGetLastError());
            return 1;
        }
        
        // Increment the message count
        message_count++;
        
        // Receive data from the server
        int recv_len = recvfrom(client_socket, recv_msg, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &server_addr_len);
        if (recv_len == SOCKET_ERROR) {
            printf("recvfrom failed with error code : %d", WSAGetLastError());
            return 1;
        }
        
        // Null-terminate the received message
        recv_msg[recv_len] = '\0';
        
        // Print the received message
        //printf("Received from server: %s\n", recv_msg);
        
        // Increment the count for the next message
        count++;
        
    }
    
    closesocket(client_socket);
    WSACleanup();
    
    return 0;
}
