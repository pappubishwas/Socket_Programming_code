// client side

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <server_ip> <server_port>\n", argv[0]);
        exit(1);
    }

    char *server_ip = argv[1];
    int server_port = atoi(argv[2]);
    int client_socket;
    struct sockaddr_in server_addr;

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        perror("Error creating client socket");
        exit(1);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0)
    {
        perror("Invalid IP address");
        close(client_socket);
        exit(1);
    }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Error connecting to server");
        close(client_socket);
        exit(1);
    }

    printf("Connected to server\n");

    char message[1024];
    while (1)
    {
        printf("Enter message to send (or 'close' to exit): ");
        fgets(message, sizeof(message), stdin);
        send(client_socket, message, strlen(message), 0);

        if (strcmp(message, "close\n") == 0)
        {
            break;
        }

        // Receive and display response
        int bytes_received = recv(client_socket, message, sizeof(message), 0);
        if (bytes_received <= 0)
        {
            break;
        }
        message[bytes_received] = '\0';
        printf("Received: %s", message);
    }

    printf("Connection closed by server\n");

    // Close socket
    close(client_socket);

    return 0;
}