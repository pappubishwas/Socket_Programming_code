#include <stdio.h> // Include the standard input/output library for printing messages
#include <stdlib.h> // Include the standard library for using exit()
#include <string.h> // Include the string library for string manipulation functions
#include <arpa/inet.h> // Include the library for using the internet address manipulation functions
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8888 // Define the port number for the server to listen on
#define MAX_MESSAGE_LEN 100 // Define the maximum length of the received message

int main() {
    int sockfd; // Declare a variable to store the socket file descriptor
    struct sockaddr_in serverAddr, clientAddr; // Declare structures to store server and client addresses
    socklen_t clientAddrLen = sizeof(clientAddr); // Declare a variable to store the size of the client address
    char buffer[MAX_MESSAGE_LEN] = {0}; // Declare a buffer to store the received message, initialized with zeros

    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // Create a UDP socket using AF_INET (IPv4) and SOCK_DGRAM (UDP)
    if (sockfd == -1) {
        perror("socket"); // Print an error message if socket creation fails
        exit(EXIT_FAILURE); // Exit the program with a failure status
    }

    // Set up the server address
    memset(&serverAddr, 0, sizeof(serverAddr)); // Initialize serverAddr to zero
    serverAddr.sin_family = AF_INET; // Set the address family to IPv4
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Bind to all available network interfaces
    serverAddr.sin_port = htons(PORT); // Convert and set the port number in network byte order

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("bind"); // Print an error message if binding fails
        close(sockfd); // Close the socket
        exit(EXIT_FAILURE); // Exit the program with a failure status
    }

    printf("Server is listening on port %d...\n", PORT); // Print a message indicating that the server is ready to listen

    while (1) { // Infinite loop to keep the server running continuously
        // Receive message from the client
        ssize_t numBytes = recvfrom(sockfd, buffer, MAX_MESSAGE_LEN - 1, 0, (struct sockaddr *)&clientAddr, &clientAddrLen); // Receive data from the client into the buffer
        if (numBytes == -1) {
            perror("recvfrom"); // Print an error message if receiving data fails
            close(sockfd); // Close the socket
            exit(EXIT_FAILURE); // Exit the program with a failure status
        }
        buffer[numBytes] = '\0'; // Null-terminate the received data as a string

        printf("Received message from client: %s\n", buffer); // Print the received message from the client

        // Reply to the client with "hello"
        const char *replyMessage = "hello"; // Create a string containing "hello"
        if (sendto(sockfd, replyMessage, strlen(replyMessage), 0, (struct sockaddr *)&clientAddr, clientAddrLen) == -1) {
            perror("sendto"); // Print an error message if sending data fails
            close(sockfd); // Close the socket
            exit(EXIT_FAILURE); // Exit the program with a failure status
        }
    }

    close(sockfd); // Close the socket
    return 0; // Exit the program with a success status
}
