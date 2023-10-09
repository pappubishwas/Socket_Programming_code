#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 12345
#define MAX_BUFFER_SIZE 1024

void receive_file(int server_socket, const char *file_name)
{
    FILE *file = fopen(file_name, "wb");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    char buffer[MAX_BUFFER_SIZE];
    int n;

    while ((n = recv(server_socket, buffer, sizeof(buffer), 0)) > 0)
    {
        if (fwrite(buffer, 1, n, file) < n)
        {
            perror("Error writing to file");
            break;
        }
    }

    fclose(file);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <server_ip>\n", argv[0]);
        exit(1);
    }

    int client_socket;
    struct sockaddr_in server_addr;

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Error creating socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Error connecting to server");
        exit(1);
    }

    printf("Connected to server at %s:%d\n", argv[1], PORT);

    // Receive the list of files from the server
    char file_list[MAX_BUFFER_SIZE];
    recv(client_socket, file_list, sizeof(file_list), 0);
    printf("Files in server's directory:\n%s", file_list);

    // Get the file name from the user
    printf("Enter the name of the file you want to download: ");
    char requested_file[MAX_BUFFER_SIZE];
    fgets(requested_file, sizeof(requested_file), stdin);
    requested_file[strcspn(requested_file, "\n")] = '\0';

    // Send the requested file name to the server
    send(client_socket, requested_file, strlen(requested_file), 0);

    // Receive and save the requested file
    receive_file(client_socket, requested_file);
    printf("File '%s' downloaded successfully.\n", requested_file);

    close(client_socket);
    return 0;
}