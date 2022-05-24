#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include "des.h"
#define PORT 9000

using namespace std;

int main(int argc,char *argv[]){

    char key[] = { '0', 'f', '1', '5', '7', '1', 'c', '9', '4', '7', 'd', '9', 'e', '8', '5', '9' };

    int sockfd, ret;
    struct sockaddr_in serverAddr;
    int newSocket;
    struct sockaddr_in newAddr;
    socklen_t addr_size;
    // buffer = message received
    char buffer[2048];
    // decrypted buffer
    char decrypted_buffer[1024];
    // message typed by server
    char message[1024];
    // encrypted message to be sent
    char encrypted_message[2048];
    pid_t childpid;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        printf("[-]Error in connection.\n");
        exit(1);
    }
    printf("[+]Server Socket is created.\n");
    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(ret < 0){
        printf("[-]Error in binding.\n");
        exit(1);
    }
    printf("[+]Bind to port is OK\n");
    if(listen(sockfd, 10) == 0){
        printf("[+]Listening....\n");
    }
    else{
        printf("[-]Error in binding.\n");
    }
    while(1){
        newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
        if(newSocket < 0){
            exit(1);
        }
        printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
        if((childpid = fork()) == 0){
            close(sockfd);
            while(1){
                recv(newSocket, buffer, 2048, 0);
                decrypt(buffer,buffer,key,2048);
                hex_to_string(buffer,decrypted_buffer,2048);
                if(strcmp(decrypted_buffer, ":exit") == 0){
                    printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
                    break;
                }
                else{

                    printf("Message from the client: %s\n",decrypted_buffer);
                    printf("Enter your message: ");
					cin.getline(message,1024);
                    string_to_hex(message,encrypted_message,2048);
                    encrypt(encrypted_message,encrypted_message,key,2048);
                    send(newSocket, encrypted_message, 2048, 0);
                    bzero(decrypted_buffer, sizeof(decrypted_buffer));
                    bzero(message, sizeof(message));
                    bzero(buffer, 2048);
                    bzero(encrypted_message, 2048);
                }
            }
        }
    }
    close(newSocket);
    exit(0);
}
