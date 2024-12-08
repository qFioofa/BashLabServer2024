#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <mutex>
#include <atomic>

#define DEFAULT_PORT 8080
#define DEFAULT_HOST "127.0.0.1"
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 1 

std::atomic<int> currentClients(0); 
std::mutex mtx; 

void handleClient(int clientSocket) {
    char buffer[BUFFER_SIZE];
    bool clientConnected = true;

    while (clientConnected) {
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesRead <= 0) {
            std::cout << "Client disconnected.\n";
            clientConnected = false; 
            break; 
        }

        std::string message(buffer);
        std::cout << "Received: " << message;

        if (message == "exit\n") {
            std::cout << "Client requested disconnection. Closing connection...\n";
            clientConnected = false;
            break; 
        } else if (message == "ping\n") {
            std::string response = "Server is running.\n";
            send(clientSocket, response.c_str(), response.size(), 0);
        } else {
            std::string response = "Message received: " + message;
            send(clientSocket, response.c_str(), response.size(), 0);
        }
    }

    
    currentClients--;
    close(clientSocket); 
    std::cout << "Client disconnected. Current clients: " << currentClients.load() << "\n";
}

int main() {
    int serverSocket;
    sockaddr_in serverAddr{};
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Socket creation failed");
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(DEFAULT_PORT);
    if (inet_pton(AF_INET, DEFAULT_HOST, &serverAddr.sin_addr) <= 0) {
        perror("Invalid host address");
        return -1;
    }

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        return -1;
    }

    if (listen(serverSocket, 5) < 0) {
        perror("Listen failed");
        return -1;
    }

    std::cout << "Server started on " << DEFAULT_HOST << ":" << DEFAULT_PORT << "\n";

    while (true) {
        sockaddr_in clientAddr{};
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientLen);
        if (clientSocket < 0) {
            perror("Failed to accept client connection");
            continue;
        }

     
        if (currentClients.load() >= MAX_CLIENTS) {
            std::cout << "Maximum number of clients reached. Rejecting new connection.\n";
            std::string errorMessage = "Server is full. Please try again later.\n";
            send(clientSocket, errorMessage.c_str(), errorMessage.size(), 0);
            close(clientSocket); 
            continue; 
        }

        
        currentClients++;
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        std::cout << "Client connected from " << clientIP << ":" << ntohs(clientAddr.sin_port) << "\n";

        
        std::thread clientThread(handleClient, clientSocket);
        clientThread.detach(); 
    }

    close(serverSocket); 
    return 0;
}
