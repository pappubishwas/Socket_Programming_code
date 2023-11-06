#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 12345
 
struct Product {
int productNumber;
char productName[100];
float productPrice;
int quantityInStock;
};
struct Product products[] = {
{1, "Product A", 10.99, 100},
{2, "Product B", 15.49, 50},
{3, "Product C", 5.99, 200},
{4, "Product D", 8.99, 75}
};
int numProducts = sizeof(products) / sizeof(products[0]);
void *handleClient(void *clientSocket) {
int *socket = (int *)clientSocket;
int productNumber;
recv(*socket, &productNumber, sizeof(productNumber), 0);
struct Product product;
for (int i = 0; i < numProducts; i++) {
if (products[i].productNumber == productNumber) {
product = products[i];
break;
}
}
send(*socket, &product, sizeof(product), 0);
close(*socket);
free(socket);
pthread_exit(NULL);
}

int main() {

int serverSocket, clientSocket;
struct sockaddr_in serverAddr, clientAddr;
socklen_t addrSize = sizeof(clientAddr);

serverSocket = socket(AF_INET, SOCK_STREAM, 0);
serverAddr.sin_family = AF_INET;
serverAddr.sin_port = htons(PORT);
serverAddr.sin_addr.s_addr = INADDR_ANY;
bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
listen(serverSocket, 5);
printf("Server listening on port %d...",PORT);

int choice;
printf("\n1 -> Using Fork\n2 -> Without Using Fork\n3 -> Using Thread\n4 -> Without Using Thread");
printf("\nEnter your choice: ");
scanf("%d",&choice);

if(choice == 1){
while (1) {
clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrSize);
if (fork() == 0) {
int productNumber;
recv(clientSocket, &productNumber, sizeof(productNumber), 0);
struct Product product;
for (int i = 0; i < numProducts; i++) {
if (products[i].productNumber == productNumber) {
product = products[i];
break;
}
}
send(clientSocket, &product, sizeof(product), 0);
close(clientSocket);
exit(1);
} else {
close(clientSocket);
}
}
}

if (choice == 2) {
while (1) {
clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrSize);
int productNumber;
recv(clientSocket, &productNumber, sizeof(productNumber), 0);
struct Product product;
for (int i = 0; i < numProducts; i++) {
if (products[i].productNumber == productNumber) {
product = products[i];
break;
}
}
send(clientSocket, &product, sizeof(product), 0);
close(clientSocket);
}
}

/*if(choice == 3) {
pthread_t thread;
while (1) {
clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrSize);
int *socketPtr = (int *)malloc(sizeof(int));
*socketPtr = clientSocket;
pthread_create(&thread, NULL, handleClient, (void *)socketPtr);
pthread_detach(thread);
}
}*/

if (choice == 4)
{
while (1) {
clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrSize);
int productNumber;
recv(clientSocket, &productNumber, sizeof(productNumber), 0);
struct Product product;
for (int i = 0; i < numProducts; i++) {
if (products[i].productNumber == productNumber) {
product = products[i];
break;
}
}
send(clientSocket, &product, sizeof(product), 0);
close(clientSocket);
}
}
return 0;
}