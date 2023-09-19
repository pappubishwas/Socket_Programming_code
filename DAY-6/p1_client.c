#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

typedef struct
{
    char roll[20];
    char name[100];
    char mobile[15];
    float cgpa;
} StudentInfo;
typedef struct
{
    char roll[20];
    char name[100];
    char mobile[15];
    float cgpa;
} StudentIn;

int main()
{
    int client_fd;
    struct sockaddr_in server_addr;

    // Create socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Connection error");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];

    while (1)
    {
        printf("1. Store Student Info\n");
        printf("2. Retrieve Student Info\n");
        printf("3. Create User\n"); // New choice
        printf("4. Exit\n");        // Updated choice
        printf("Enter choice: ");
        int choice;
        scanf("%d", &choice);

        if (choice == 1)
        {
            // Send command to server
            const char *command = "store";
            if (send(client_fd, command, strlen(command), 0) == -1)
            {
                perror("Sending command failed");
                exit(EXIT_FAILURE);
            }

            // Input student info
            StudentInfo student;
            printf("Enter student info (Roll Name Mobile CGPA): \n");
            scanf("%s %s %s %f", student.roll, student.name, student.mobile, &student.cgpa);

            // Send student info to server
            if (send(client_fd, &student, sizeof(StudentInfo), 0) == -1)
            {
                perror("Sending student info failed");
                exit(EXIT_FAILURE);
            }

            // Receive response from server
            ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
            if (bytes_received == -1)
            {
                perror("Receiving response failed");
                exit(EXIT_FAILURE);
            }
            buffer[bytes_received] = '\0';

            // Print server response
            printf("Server response: %s\n", buffer);
        } // ...
        else if (choice == 2)
        {
            const char *command = "retrieve";
            if (send(client_fd, command, strlen(command), 0) == -1)
            {
                perror("Sending command failed");
                exit(EXIT_FAILURE);
            }

            // Input login credentials
            char username[50], password[50];
            printf("Enter username: ");
            scanf("%s", username);
            printf("Enter password: ");
            scanf("%s", password);

            // Send username and password to server
            send(client_fd, username, strlen(username), 0);
            send(client_fd, password, strlen(password), 0);

            // Receive and print server response
            memset(buffer, 0, sizeof(buffer));
            ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
            if (bytes_received == -1)
            {
                perror("Receiving response failed");
                exit(EXIT_FAILURE);
            }
            buffer[bytes_received] = '\0';

            printf("Server response: %s\n", buffer);

            if (strcmp(buffer, "Authentication successful") == 0)
            {
                char studentRoll[20];
                printf("Enter the student roll to retrieve details: ");
                scanf("%s", studentRoll);

                // Send the student roll to the server
                if (send(client_fd, studentRoll, strlen(studentRoll), 0) == -1)
                {
                    perror("Sending student roll failed");
                    exit(EXIT_FAILURE);
                }

                // Receive server's response
                memset(buffer, 0, sizeof(buffer));
                bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
                if (bytes_received == -1)
                {
                    perror("Receiving response failed");
                    exit(EXIT_FAILURE);
                }
                buffer[bytes_received] = '\0';

                // Check the response to determine if the server found the student
                if (strcmp(buffer, "Student not found") == 0)
                {
                    printf("Student with roll %s not found.\n", studentRoll);
                }
                else
                {
                    // Receive and print student details
                    StudentIn student;
                    memset(buffer, 0, sizeof(buffer));
                    bytes_received = recv(client_fd, &student, sizeof(StudentIn), 0);
                    if (bytes_received == -1)
                    {
                        perror("Receiving student info failed");
                        exit(EXIT_FAILURE);
                    }

                    printf("Student Info:\n");
                    printf("Roll: %s\nName: %s\nMobile: %s\nCGPA: %.2f\n",
                           student.roll, student.name, student.mobile, student.cgpa);
                }
            }
        }

        else if (choice == 3)
        {
            const char *command = "register"; // New command
            if (send(client_fd, command, strlen(command), 0) == -1)
            {
                perror("Sending command failed");
                exit(EXIT_FAILURE);
            }

            // Input username and password for new user or admin
            char newUsername[50];
            char newPassword[50];
            printf("Enter new username: ");
            scanf("%s", newUsername);
            printf("Enter new password: ");
            scanf("%s", newPassword);

            // Concatenate newUsername and newPassword into a single string
            char credentials[100]; // Adjust the size as needed
            snprintf(credentials, sizeof(credentials), "%s %s", newUsername, newPassword);

            // Send the concatenated credentials to the server in one send call
            send(client_fd, credentials, strlen(credentials), 0);

            // Receive response from server
            ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
            if (bytes_received == -1)
            {
                perror("Receiving response failed");
                exit(EXIT_FAILURE);
            }
            buffer[bytes_received] = '\0';

            // Print server response
            printf("Server response: %s\n", buffer);
        }
        else if (choice == 4)
        {
            const char *command = "exit";
            if (send(client_fd, command, strlen(command), 0) == -1)
            {
                perror("Sending exit command failed");
            }
            printf("Exiting...\n");
            break;
        }
    }

    close(client_fd);

    return 0;
}
