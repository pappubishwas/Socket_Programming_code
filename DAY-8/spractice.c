#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

int main(int argc,char *argv[]){
    if(argc!=2){
        fprintf(stderr, "Usage %s <port> \n", argv[0]);
        exit(1);
    }
    //declare variable
    int port = atoi(argv[1]);
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);


    //create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket==-1){
        perror("Error creating server socket");
        exit(1);
    }
    
    //bind
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr_s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if(bind(server_socket,(struct sockaddr *)&server_addr,sizeof(server_addr))==-1){
        perror("Error in binding");
        close(server_socket);
        exit(1);
    }

    //Listen
    if(listen(server_socket,5)==-1){
        perror("Error in listening");
        close(server_socket);
        exit(1);
    }

    printf("Server listening on port %d ....\n", port);
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
    
}