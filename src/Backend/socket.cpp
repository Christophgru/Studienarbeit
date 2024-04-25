#include "socket.h"



#include <iostream>
#include <vector>
#include <string>





SocketVerwaltung::SocketVerwaltung(){
    
   WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock\n";
        return ;
    }
    
    
    
    

    //for all sockets init
    initSocket("127.0.0.1", 12345);
    // Close the client socket
    //closesocket(clientSocket);
}

int SocketVerwaltung::initSocket(std::string ip, int port){
    // Create a client socket
    
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

    std::cout << "Connected to Datasource!" << std::endl;

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
    std::cout<<"connected to client supp with no err:"<<WSAGetLastError();
    return 0;
}

/*returns numbers of bytes read*/
    int SocketVerwaltung::read(std::string* s){
        
        
        for (SOCKET clientSocket : clientSockets) {
        
            // Dynamically allocate memory to store received data
            int bytesRead = recv(clientSocket, buffer, BUFSIZE, 0);
        

            if (bytesRead > 0) {
                std::cout << "Received data from client:" << buffer << std::endl;
                number_datepoints++;
                s->assign(buffer);
                std::cout<<".";
                //std::cout<<" "<<bytesRead<<" Bytes read";
                if(bytesRead>BUFSIZE){
                    printf("\ndouble frame received, throwing away\n");//or maybe only send first half?
                    s->assign("");
                    memset(buffer, 0, BUFSIZE);
                }else
                
                    memset(buffer, 0, BUFSIZE);
                    return bytesRead;
                    
            }else if (bytesRead==0)
            {
                printf("no data rec");
            }else{
                // printf("error");
                //throw;
                std::cerr << "Read failed, Err #" << WSAGetLastError() << std::endl;
                if(WSAGetLastError()!=10014)
                    clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), clientSocket), clientSockets.end());
                    
            }
        }

        if(clientSockets.size()==0){
            printf("Connection lost\n");
            //try reinit for all, if one succeeds cintinue, else kill programm
            int status=initSocket("127.0.0.1", 12345);
            if(status==1){
                printf("Fatal Error, Server Connection lost. shutting down");
                return -1;
                }
            else if(status==0){
                 printf("Reconnection success.\n");
                return read(s);
            }
            else {
                printf("Reconnection unsuccessfull.\n");
                return 0;
                }
        }
        
        return 0;
    }
    