#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstring>
#include <arpa/inet.h> 
#include <unistd.h>


int connectToServer(const std::string& serverAddress, int port) {
    int clientSocket;

    while (true) {
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == -1) {
            std::cerr << "Error creating socket" << std::endl;
            continue;
        }

        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);

        
        if (inet_pton(AF_INET, serverAddress.c_str(), &serverAddr.sin_addr) <= 0) {
            std::cerr << "Invalid address/Address not supported" << std::endl;
            close(clientSocket);
            continue;
        }

        
        if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == 0) {
            return clientSocket;
        } else {
            std::cerr << "Unable to connect to server, retrying in 5 seconds..." << std::endl;
            close(clientSocket);
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }
}

int main() {
    const std::string serverAddress = "127.0.0.1"; 
    const int port = 12345; 

    
    int clientSocket = connectToServer(serverAddress, port);
    std::cout << "Connected to server!" << std::endl;

    while (true) {
        std::string input;
        std::cout << "Enter command (ping, exit, connect): ";
        std::getline(std::cin, input);

        if (input == "exit") {
            
            send(clientSocket, (input + "\n").c_str(), input.size() + 1, 0);
            std::cout << "Exiting client..." << std::endl;
            close(clientSocket);
            break;
        } else if (input == "connect") {
            
            std::cout << "Attempting to reconnect..." << std::endl;
            close(clientSocket);
            clientSocket = connectToServer(serverAddress, port);
            std::cout << "Reconnected to server!" << std::endl;
        } else {
            
            send(clientSocket, (input + "\n").c_str(), input.size() + 1, 0);

            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));

            
            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
            if (bytesReceived > 0) {
                std::cout << "Response from server: " << buffer << std::endl;
            } else {
                std::cerr << "Error receiving from server. Attempting to reconnect..." << std::endl;
                close(clientSocket);
                clientSocket = connectToServer(serverAddress, port);
                std::cout << "Reconnected to server!" << std::endl;
            }
        }
    }

    close(clientSocket);
    return 0;
}
