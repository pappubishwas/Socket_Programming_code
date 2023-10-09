// server

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int port = atoi(argv[1]);
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("Error creating server socket");
        exit(1);
    }

    // Bind
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Error binding");
        close(server_socket);
        exit(1);
    }

    // Listen
    if (listen(server_socket, 5) == -1)
    {
        perror("Error listening");
        close(server_socket);
        exit(1);
    }

    printf("Server listening on port %d...\n", port);

    // Accept a connection
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
    if (client_socket == -1)
    {
        perror("Error accepting connection");
        close(server_socket);
        exit(1);
    }

    printf("Connection established with client\n");

    // Exchange messages
    
    char buffer[1024];
    while (1)
    {
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0)
        {
            break;
        }
        buffer[bytes_received] = '\0';
        printf("Received: %s", buffer);

        // Send a response
        char response[1024];
        printf("Enter message to send (or 'close' to exit): ");
        fgets(response, sizeof(response), stdin);
        send(client_socket, response, strlen(response), 0);

        if (strcmp(response, "close\n") == 0)
        {
            break;
        }
    }

    printf("Connection closed by client\n");

    // Close sockets
    close(client_socket);
    close(server_socket);

    return 0;
}