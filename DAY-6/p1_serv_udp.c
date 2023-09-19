#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>

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
    char username[50];
    char password[50];
} userInfo;

int main()
{
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    StudentInfo students[100]; // Maximum of 100 students
    userInfo users[100];       // Maximum of 100 users
    int numStudents = 0;
    int numUsers = 0;

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to address
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    char buffer[BUFFER_SIZE];

    while (1)
    {
        ssize_t bytes_received = recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_len);
        if (bytes_received == -1)
        {
            perror("Receiving failed");
            exit(EXIT_FAILURE);
        }
        buffer[bytes_received] = '\0';

        if (strcmp(buffer, "store") == 0)
        {
            // Receive student info from client
            bytes_received = recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_len);
            if (bytes_received == -1)
            {
                perror("Receiving student info failed");
                exit(EXIT_FAILURE);
            }
            buffer[bytes_received] = '\0';

            // Parse student info and store
            sscanf(buffer, "%s %s %s %f", students[numStudents].roll, students[numStudents].name,
                   students[numStudents].mobile, &students[numStudents].cgpa);

            numStudents++;
            printf("Student info stored\n");
            const char *response = "Student info stored successfully";
            sendto(server_fd, response, strlen(response), 0, (struct sockaddr *)&client_addr, addr_len);
        }
        // ...

        else if (strcmp(buffer, "register") == 0)
        {
            // Receive new username and password from the client
            bytes_received = recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_len);
            if (bytes_received == -1)
            {
                perror("Receiving registration info failed");
                exit(EXIT_FAILURE);
            }
            buffer[bytes_received] = '\0';

            // Parse new username and password
            int parsed_items = sscanf(buffer, "%s %s", users[numUsers].username, users[numUsers].password);

            if (parsed_items == 2)
            {
                printf("New user registered: %s\n", users[numUsers].username);
                numUsers++;
                const char *response = "Registration successful";
                sendto(server_fd, response, strlen(response), 0, (struct sockaddr *)&client_addr, addr_len);
            }
            else
            {
                const char *response = "Invalid registration info format";
                sendto(server_fd, response, strlen(response), 0, (struct sockaddr *)&client_addr, addr_len);
            }
        }
        // ...
        else if (strcmp(buffer, "retrieve") == 0)
        {
            char username[50];
            char password[50];

            // Receive username from the client
            ssize_t bytes_received = recvfrom(server_fd, username, sizeof(username) - 1, 0, (struct sockaddr *)&client_addr, &addr_len);
            if (bytes_received == -1)
            {
                perror("Receiving username failed");
                exit(EXIT_FAILURE);
            }
            username[bytes_received] = '\0';

            // Receive password from the client
            bytes_received = recvfrom(server_fd, password, sizeof(password) - 1, 0, (struct sockaddr *)&client_addr, &addr_len);
            if (bytes_received == -1)
            {
                perror("Receiving password failed");
                exit(EXIT_FAILURE);
            }
            password[bytes_received] = '\0';

            // Perform authentication (replace with your own authentication logic)
            bool authenticated = false;
            for (int i = 0; i < numUsers; i++)
            {
                if (strcmp(username, users[i].username) == 0 && strcmp(password, users[i].password) == 0)
                {
                    authenticated = true;
                    break;
                }
            }

            // Send authentication result to client
            const char *auth_result = authenticated ? "Authentication successful" : "Authentication failed";
            sendto(server_fd, auth_result, strlen(auth_result), 0, (struct sockaddr *)&client_addr, addr_len);

            if (authenticated)
            {
                char studentRoll[20];
                ssize_t bytes_received = recvfrom(server_fd, studentRoll, sizeof(studentRoll) - 1, 0, (struct sockaddr *)&client_addr, &addr_len); // Receive student roll
                if (bytes_received == -1)
                {
                    perror("Receiving student roll failed");
                    exit(EXIT_FAILURE);
                }
                studentRoll[bytes_received] = '\0';
                printf("Received student roll: %s\n", studentRoll);

                // Search for the student with the provided roll in the students array
                bool studentFound = false;
                for (int i = 0; i < numStudents; i++)
                {
                    if (strcmp(studentRoll, students[i].roll) == 0)
                    {
                        // Send student details to the client
                        if (sendto(server_fd, &students[i], sizeof(StudentInfo), 0, (struct sockaddr *)&client_addr, addr_len) == -1)
                        {
                            perror("Sending student info failed");
                            exit(EXIT_FAILURE);
                        }
                        studentFound = true;
                        break;
                    }
                }

                if (!studentFound)
                {
                    const char *notFoundMessage = "Student not found";
                    if (sendto(server_fd, notFoundMessage, strlen(notFoundMessage), 0, (struct sockaddr *)&client_addr, addr_len) == -1)
                    {
                        perror("Sending 'Student not found' message failed");
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }

        // exit ...

        else if (strcmp(buffer, "exit") == 0)
        {
            printf("Client disconnected\n");
            break;
        }
    }
    close(server_fd);

    return 0;
}