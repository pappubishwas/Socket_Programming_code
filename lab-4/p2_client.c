#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#define SERVER_IP "127.0.0.1"
#define PORT 12340
#define BUFFER_SIZE 1024

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    
    // Create socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    
    // Connect to server
    connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    char message[BUFFER_SIZE];
    
    while (1) {
        printf("Enter a message (type 'exit' to quit): ");
        fgets(message, sizeof(message), stdin);
        
        // Remove the newline character from the input
        message[strcspn(message, "\n")] = '\0';
        
        // Send message to server
        send(client_fd, message, strlen(message), 0);
        
        // Check if the message is "EXIT"
        if (strcmp(message, "exit") == 0) {
            break;
        }
        
        char buffer[BUFFER_SIZE];
        
        // Receive response from server
        recv(client_fd, buffer, sizeof(buffer), 0);
        
        // Print response from server
        printf("Server response: %s\n", buffer);
    }
    
    close(client_fd);
    
    return 0;}