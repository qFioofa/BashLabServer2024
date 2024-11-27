#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define DEFAULT_PORT 8080
#define DEFAULT_HOST "127.0.0.1"
#define BUFFER_SIZE 1024

bool serverRunning = false;
int serverSocket = -1;
int clientSocket = -1;

void cleanup() {
    if (clientSocket != -1) close(clientSocket);
    if (serverSocket != -1) close(serverSocket);
}

bool launchServer(const std::string& host, int port) {
    sockaddr_in serverAddr{};
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Socket creation failed");
        return false;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr) <= 0) {
        perror("Invalid host address");
        return false;
    }

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        return false;
    }

    if (listen(serverSocket, 1) < 0) {
        perror("Listen failed");
        return false;
    }

    std::cout << "Server started on " << host << ":" << port << "\n";
    serverRunning = true;
    return true;
}

void handleClient() {
    char buffer[BUFFER_SIZE];

    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesRead <= 0) {
            std::cout << "Client disconnected.\n";
            close(clientSocket);
            clientSocket = -1;
            return;
        }

        std::string message(buffer);
        std::cout << "Received: " << message;

        if (message == "exit\n") {
            std::cout << "Client requested disconnection. Closing connection...\n";
            close(clientSocket);
            clientSocket = -1;
            return;
        } else if (message == "ping\n") {
            std::string response = serverRunning ? "Server is running.\n" : "Server is not running.\n";
            send(clientSocket, response.c_str(), response.size(), 0);
        } else {
            std::string response = "Message received: " + message;
            send(clientSocket, response.c_str(), response.size(), 0);
        }
    }
}

int main() {
std::string host = DEFAULT_HOST;
    int port = DEFAULT_PORT;

    if (!serverRunning && launchServer(host, port)) {
        std::cout << "Waiting for client to connect...\n";

        while (true) {
            sockaddr_in clientAddr{};
            socklen_t clientLen = sizeof(clientAddr);

            clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientLen);
            if (clientSocket < 0) {
                perror("Failed to accept client connection");
                continue;
            }

            char clientIP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
            std::cout << "Client connected from " << clientIP << ":" << ntohs(clientAddr.sin_port) << "\n";

            handleClient();
            std::cout << "Waiting for a new client...\n";
        }
    }

    return 0;
}