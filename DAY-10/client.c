#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

struct Product {
int productNumber;
char productName[100];
float productPrice;
int quantityInStock;
};

int main() {
int clientSocket;
struct sockaddr_in serverAddr;

clientSocket = socket(AF_INET, SOCK_STREAM, 0);
serverAddr.sin_family = AF_INET;
serverAddr.sin_port = htons(12345);
serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
perror("Connection failed");
return 1;
}

int productNumber;
printf("Enter the product number: ");
scanf("%d", &productNumber);
send(clientSocket, &productNumber, sizeof(productNumber), 0);
struct Product product;
recv(clientSocket, &product, sizeof(product), 0);
if (product.productNumber == 0) {
printf("Product not found.\n");
} else {
printf("Product Number: %d\n", product.productNumber);
printf("Product Name: %s\n", product.productName);
printf("Product Price: %.2f\n", product.productPrice);
printf("Quantity in Stock: %d\n", product.quantityInStock);
}
close(clientSocket);

return 0;
}