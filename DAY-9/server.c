#include <arpa/inet.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 12345
#define MAX_BUFFER_SIZE 1024

void send_file(int client_socket, const char *file_name)
{
    FILE *file = fopen(file_name, "rb");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    char buffer[MAX_BUFFER_SIZE];
    int n;

    while ((n = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        if (send(client_socket, buffer, n, 0) == -1)
        {
            perror("Error sending file");
            break;
        }
    }

    fclose(file);
}

int main(int argc, char *argv[])
{
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Error creating socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Error binding");
        exit(1);
    }

    if (listen(server_socket, 5) == -1)
    {
        perror("Error listening");
        exit(1);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1)
    {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);

        if (client_socket == -1)
        {
            perror("Error accepting connection");
            continue;
        }

        printf("Client connected from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Send a list of files in the current directory
        DIR *dir;
        struct dirent *entry;

        if ((dir = opendir(".")) != NULL)
        {
            char file_list[MAX_BUFFER_SIZE] = "";

            while ((entry = readdir(dir)) != NULL)
            {
                strcat(file_list, entry->d_name);
                strcat(file_list, "\n");
            }

            closedir(dir);
            send(client_socket, file_list, strlen(file_list), 0);
        }
        else
        {
            perror("Error reading directory");
        }

        // Receive the file name requested by the client
        char requested_file[MAX_BUFFER_SIZE];
        recv(client_socket, requested_file, sizeof(requested_file), 0);

        // Send the requested file
        send_file(client_socket, requested_file);

        // Close the client socket
        close(client_socket);
        printf("Client disconnected.\n");
    }

    close(server_socket);
    return 0;
}