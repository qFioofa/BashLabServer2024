#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h> 
#include <errno.h> 

#define PORT 1222
#define BUFFER_SIZE 1024
#define TEST_IP "8.8.8.8" 

int check_internet_connection() {
    struct sockaddr_in server_addr;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 0; 
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(853); 
    server_addr.sin_addr.s_addr = inet_addr(TEST_IP); 


    int result = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    close(sockfd);

    return result == 0; 
}

int main() {
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(server_addr);


    if (!check_internet_connection()) {
        fprintf(stderr, "Error: No internet connection.\n");
        exit(1);
    }

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;


    while (1) {
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *)&server_addr, addr_len);

        if (strcmp(buffer, "exit") == 0) {
            break;
        }

        int n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len);
        buffer[n] = '\0';
        printf("Server response: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
