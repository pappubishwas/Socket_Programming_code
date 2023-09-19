#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
    int server_socket;
    struct sockaddr_in server_addr;

    char buffer[1024];

    // Create socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port 8080 (UDP)...\n");

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        ssize_t bytes_received = recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_len);
        if (bytes_received == -1) {
            perror("Receiving failed");
            exit(EXIT_FAILURE);
        }

        printf("Received : %s", buffer);

        ssize_t bytes_sent = sendto(server_socket, buffer, bytes_received, 0, (struct sockaddr*)&client_addr, client_len);
        if (bytes_sent == -1) {
            perror("Sending failed");
            exit(EXIT_FAILURE);
        }
    }

    // Close socket
    close(server_socket);

    return 0;
}
