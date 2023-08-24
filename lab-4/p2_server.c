#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 12340
#define BUFFER_SIZE 1024

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to address
    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Listen for connections
    listen(server_fd, 1);

    printf("Server listening on port %d...\n", PORT);

    // Accept incoming connection
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);

    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE];
    while (1)
    {
        // Receive message from client
        recv(client_fd, buffer, sizeof(buffer), 0);

        printf("Received message from client: %s\n", buffer);

        // Check if the message is "EXIT"
        if (strcmp(buffer, "exit") == 0)
        {
            const char *response = "Server exiting...";
            send(client_fd, response, strlen(response), 0);
            break;
        }
        printf("Enter a message (type 'exit' to quit): ");
        fgets(message, sizeof(message), stdin);

        // Remove the newline character from the input
        message[strcspn(message, "\n")] = '\0';
        //const char *response = message;
        send(client_fd, message, strlen(message), 0);

        if (strcmp(message, "exit") == 0)
        {
            break;
        }
        // Send response to client
        // const char *response = message;
        // send(client_fd, response, strlen(response), 0);
    }

    close(client_fd);
    close(server_fd);

    return 0;
}