#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <cstring>
#include <arpa/inet.h> 
#include <unistd.h>    


void handleClient(int clientSocket, sockaddr_in clientAddr) {
    char buffer[1024];
    std::string welcomeMessage = "Welcome to the C++ server\n";

    
    send(clientSocket, welcomeMessage.c_str(), welcomeMessage.size(), 0);

    
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
    int clientPort = ntohs(clientAddr.sin_port);

    std::cout << "Client connected: " << clientIP << ":" << clientPort << std::endl;

    while (true) {
        
        memset(buffer, 0, sizeof(buffer));

        
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

        if (bytesReceived > 0) {
            std::string message(buffer);
            std::cout << "Received from client " << clientIP << ":" << clientPort << ": " << message;

            if (message == "ping\n") {
                std::string response = "Server is alive\n";
                send(clientSocket, response.c_str(), response.size(), 0);
            } else if (message == "exit\n") {
                std::string response = "Disconnecting...\n";
                send(clientSocket, response.c_str(), response.size(), 0);
                std::cout << "Client disconnected: " << clientIP << ":" << clientPort << std::endl;
                close(clientSocket);
                break;
            } else {
                std::string response = "Unknown command\n";
                send(clientSocket, response.c_str(), response.size(), 0);
            }
        } else {
            std::cout << "Client disconnected: " << clientIP << ":" << clientPort << std::endl;
            close(clientSocket);
            break;
        }
    }
}

int main() {
    
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(12345); 

    
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        close(serverSocket);
        return 1;
    }

    
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error listening on socket" << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Server started, waiting for connections..." << std::endl;

    while (true) {
        
        sockaddr_in clientAddr{};
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);

        if (clientSocket == -1) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        
        std::thread clientThread(handleClient, clientSocket, clientAddr);
        clientThread.detach(); 
    }

    close(serverSocket);
    return 0;
}
