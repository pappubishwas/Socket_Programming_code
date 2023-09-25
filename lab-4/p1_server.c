#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    
    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    // Bind socket to address
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }
    
    // Listen for connections
    if (listen(server_fd, 1) == -1) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Server listening on port %d...\n", PORT);
    
    // Accept incoming connection
    if ((client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len)) == -1) {
        perror("Acceptance failed");
        exit(EXIT_FAILURE);
    }
    
    char buffer[BUFFER_SIZE];
    
    // Receive message from client
    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
    if (bytes_received == -1) {
        perror("Receiving failed");
        exit(EXIT_FAILURE);
    }
    
    // Print received message
    printf("Received message from client: %s\n", buffer);
    
    // Send response to client
    const char* response = "Received Message";
    if (send(client_fd, response, strlen(response), 0) == -1) {
        perror("Sending response failed");
        exit(EXIT_FAILURE);
    }
    
    close(client_fd);
    close(server_fd);
    
    return 0;
}