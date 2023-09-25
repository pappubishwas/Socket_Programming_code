#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    char buffer[1024];

    // Create socket
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Change to server's IP

    while (1) {
        printf("Enter message: ");
        fgets(buffer, sizeof(buffer), stdin);

        // Send message to server
        ssize_t bytes_sent = sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
        if (bytes_sent == -1) {
            perror("Sending failed");
            exit(EXIT_FAILURE);
        }

        memset(buffer, 0, sizeof(buffer));

        // Receive message from server
        ssize_t bytes_received = recvfrom(client_socket, buffer, sizeof(buffer), 0, NULL, NULL);
        if (bytes_received == -1) {
            perror("Receiving failed");
            exit(EXIT_FAILURE);
        }

        printf("Received from server: %s", buffer);
    }

    // Close socket
    close(client_socket);

    return 0;
}
