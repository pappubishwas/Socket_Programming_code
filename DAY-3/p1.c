#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8888
#define MAX_MESSAGE_LEN 100
int main()
{
    int sockfd;
    struct sockaddr_in serverAddr, cilentAddr;

    socklen_t cilentAddrLen = sizeof(cilentAddr);
    char buffer[MAX_MESSAGE_LEN] = {0};

    // Create a UDP coket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    // Set up the server address

    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_addr.s_addr=INADDR_ANY;
    serverAddr.sin_port=htons(PORT);


    //Bind the socket to the server address

    if(bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr))==-1){
        perror("Bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }


    printf("Server is listening on port %d....\n", PORT);

    while (1)
    {
        //Receive message from the client
        ssize_t numBytes=recvfrom(sockfd,buffer,MAX_MESSAGE_LEN-1,0,(struct sockaddr*)&cilentAddr,&cilentAddrLen);
        if(numBytes==-1){
            perror("recvfrom");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        buffer[numBytes]='\0';

        printf("Received message from cilent :%s\n",buffer);

        // Reply to the client with "Hello"

        const char *replyMessage="Hello";
        if(sendto(sockfd,replyMessage,strlen(replyMessage),0,(struct sockaddr*)&cilentAddr,cilentAddrLen)==-1){
            perror("sendto");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
    }
    
    close(sockfd);

    return 0;

    //     struct sockaddr_in server_addr;
    // server_addr.sin_family = AF_UNIX;

    // server_addr.sin_port = htons(2000);

    // server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // bind(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // listen(socket_desc, 5);
}
