#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "client_server_handler.h"

#define PORT 1222
#define BUFFER_SIZE 1024

int main() {
int server_fd;
struct sockaddr_in server_addr, client_addr;
socklen_t addr_len = sizeof(client_addr);
char buffer[BUFFER_SIZE];

server_fd = socket(AF_INET, SOCK_DGRAM, 0);
if (server_fd < 0) {
perror("Socket creation failed");
exit(1);
}

memset(&server_addr, 0, sizeof(server_addr));
server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = INADDR_ANY;
server_addr.sin_port = htons(PORT);

if (bind(server_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
perror("Bind failed");
close(server_fd);
exit(1);
}

while (1) {
int n = recvfrom(server_fd, (char *)buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
buffer[n] = '\0';
printf("Received: %s\n", buffer);

if (strcmp(buffer, "ping") == 0) {
sendto(server_fd, "pong", strlen("pong"), 0, (struct sockaddr *)&client_addr, addr_len);
}
else if (strcmp(buffer, "exit") == 0) {
break;
}
else {
const char *error_message = "Wrong message";
sendto(server_fd, error_message, strlen(error_message), 0, (struct sockaddr *)&client_addr, addr_len);
printf("Error: Wrong message '%s'\n", buffer);
}
}

close(server_fd);
return 0;
}
