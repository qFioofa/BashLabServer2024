#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstring>
#include <arpa/inet.h> 
#include <unistd.h>
#include <vector>

void showInstructions() {
    std::cout << "Available commands:" << std::endl;
    std::cout << "1. ping - sends a request to the server and gets the server status." << std::endl;
    std::cout << "2. exit - closes the application." << std::endl;
    std::cout << "3. any other message - just sends text." << std::endl;
}

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
        }
        else {
            std::cerr << "Unable to connect to server at " << serverAddress << ":" << port
                << ", retrying in 5 seconds..." << std::endl;
            close(clientSocket);
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }
}

void sendPendingRequests(int clientSocket, std::vector<std::string>& pendingRequests) {
    for (const auto& request : pendingRequests) {
        send(clientSocket, (request + "\n").c_str(), request.size() + 1, 0);
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));

        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0) {
            std::cout << "Response from server: " << buffer << std::endl;
        }
        else {
            std::cerr << "Error receiving from server. Request will be retried later." << std::endl;
            break;
        }
    }
    pendingRequests.clear();
}
int main() {
    const std::string serverAddress = "127.0.0.1";
    const int port = 8080;

    showInstructions();

    int clientSocket = connectToServer(serverAddress, port);
    std::cout << "Connected to server at " << serverAddress << ":" << port << "!" << std::endl;

    std::vector<std::string> pendingRequests;

    while (true) {
        std::string input;
        std::cout << "Enter command (ping, exit): ";
        std::getline(std::cin, input);

        if (input == "exit") {
            send(clientSocket, (input + "\n").c_str(), input.size() + 1, 0);
            std::cout << "Exiting client..." << std::endl;
            close(clientSocket);
            break;
        }
        else if (input == "ping") {
            send(clientSocket, (input + "\n").c_str(), input.size() + 1, 0);

            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));

            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
            if (bytesReceived > 0) {
                std::cout << "Response from server: " << buffer << std::endl;
            }
            else {
                std::cerr << "Error receiving from server. Saving request for later..." << std::endl;
                pendingRequests.push_back(input);
                close(clientSocket);
                clientSocket = connectToServer(serverAddress, port);
                std::cout << "Reconnected to server!" << std::endl;
                sendPendingRequests(clientSocket, pendingRequests);
            }
        }
        else {
            if (input.size() > 1024) {
                input = input.substr(0, 1024);
                std::cerr << "Message was too long and has been truncated." << std::endl;
            }

            send(clientSocket, (input + "\n").c_str(), input.size() + 1, 0);

            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));

            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
            if (bytesReceived > 0) {
                std::cout << "Response from server: " << buffer << std::endl;
            }
            else {
                std::cerr << "Error receiving from server. Saving request for later..." << std::endl;
                pendingRequests.push_back(input);
                close(clientSocket);
                clientSocket = connectToServer(serverAddress, port);
                std::cout << "Reconnected to server!" << std::endl;

                sendPendingRequests(clientSocket, pendingRequests);
            }
        }
    }

    close(clientSocket);
    return 0;
}
