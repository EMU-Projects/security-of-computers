#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "des.h"
#include <iostream>
#define PORT 9000

using namespace std;

int main(){

    char key[] = { '0', 'f', '1', '5', '7', '1', 'c', '9', '4', '7', 'd', '9', 'e', '8', '5', '9' };

	int clientSocket, ret;
	struct sockaddr_in serverAddr;
    // buffer = message received
    char buffer[2048];
    // decrypted buffer
    char decrypted_buffer[1024];
    // message typed by server
    char message[1024];
    // encrypted message to be sent
    char encrypted_message[2048];
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");

	while(1){
		printf("Enter your message: ");
		cin.getline(message,1024);
		string_to_hex(message,encrypted_message,2048);
		encrypt(encrypted_message,encrypted_message,key,2048);
		send(clientSocket, encrypted_message, 2048, 0);
        bzero(message, sizeof(message));

		if(strcmp(buffer, ":exit") == 0){
			close(clientSocket);
			printf("[-]Disconnected from server.\n");
			exit(1);
		}

		if(recv(clientSocket, buffer, 2048, 0) < 0){
			printf("[-]Error in receiving data.\n");
		}else{
			decrypt(buffer,buffer,key,2048);
			hex_to_string(buffer,decrypted_buffer,2048);
			printf("Message from the Server: %s\n", decrypted_buffer);
            bzero(buffer, sizeof(buffer));
            bzero(message, sizeof(message));
		}
	}
	return 0;
}
