#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    
    // Create socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }
    
    // Connect to server
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
    
    const char* message = "Roll no -2106287";
    
    // Send message to server
    if (send(client_fd, message, strlen(message), 0) == -1) {
        perror("Sending message failed");
        exit(EXIT_FAILURE);
    }
    
    char buffer[BUFFER_SIZE];
    
    // Receive response from server
    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
    if (bytes_received == -1) {
        perror("Receiving response failed");
        exit(EXIT_FAILURE);
    }
    
    // Print response from server
    printf("Response from server: %s\n", buffer);
    
    close(client_fd);
    
    return 0;
}