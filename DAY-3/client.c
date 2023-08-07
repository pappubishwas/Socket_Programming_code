#include <stdio.h> // Include the standard input/output library for printing messages
#include <stdlib.h> // Include the standard library for using exit()
#include <string.h> // Include the string library for string manipulation functions
#include <arpa/inet.h> // Include the library for using the internet address manipulation functions
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>


#define SERVER_IP "127.0.0.1" // Define the server IP address
#define PORT 4444 // Define the port number for communication
#define MAX_MESSAGE_LEN 100 // Define the maximum length of the message

int main() {
    int sockfd; // Declare a variable to store the socket file descriptor
    struct sockaddr_in serverAddr; // Declare a structure to store the server address

    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // Create a UDP socket using AF_INET (IPv4) and SOCK_DGRAM (UDP)
    if (sockfd == -1) {
        perror("socket"); // Print an error message if socket creation fails
        exit(EXIT_FAILURE); // Exit the program with a failure status
    }

    // Set up the server address
    memset(&serverAddr, 0, sizeof(serverAddr)); // Initialize serverAddr to zero
    serverAddr.sin_family = AF_INET; // Set the address family to IPv4
    serverAddr.sin_port = htons(PORT); // Convert and set the port number in network byte order
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) { // Convert the server IP address from text to binary format
        perror("inet_pton"); // Print an error message if the conversion fails
        close(sockfd); // Close the socket
        exit(EXIT_FAILURE); // Exit the program with a failure status
    }

    // Send "hello" message to the server
    const char *message = "hello"; // Create a string containing "hello"
    if (sendto(sockfd, message, strlen(message), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("sendto"); // Print an error message if sending data fails
        close(sockfd); // Close the socket
        exit(EXIT_FAILURE); // Exit the program with a failure status
    }

    char buffer[MAX_MESSAGE_LEN] = {0}; // Declare a buffer to store the received message, initialized with zeros
    // Receive the reply from the server
    ssize_t numBytes = recvfrom(sockfd, buffer, MAX_MESSAGE_LEN - 1, 0, NULL, NULL); // Receive data from the server into the buffer
    if (numBytes == -1) {
        perror("recvfrom"); // Print an error message if receiving data fails
        close(sockfd); // Close the socket
        exit(EXIT_FAILURE); // Exit the program with a failure status
    }
    buffer[numBytes] = '\0'; // Null-terminate the received data as a string

    printf("Received reply from server: %s\n", buffer); // Print the received message from the server

    close(sockfd); // Close the socket
    return 0; // Exit the program with a success status
}
