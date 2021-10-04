#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX 80
#define PORT 2525
#define SA struct sockaddr
void func(int sockfd) {
	int bytes, total;
	uint8_t lala;
	uint16_t lala16;
	uint32_t lala32;
	char buff[MAX];
	for (;;) {
		bzero(buff, sizeof(buff));
		printf("Enter\n");
		getchar(); // Presionar una tecla para enviar el dato
		lala = 200;
		lala16 = 1000;
		lala32 = 1234567;
		buff[0] = lala;
		lala16 = htons(lala16);
		lala32 = htonl(lala32);
		memcpy(&(buff[1]),&lala16,2);
		memcpy(&(buff[3]),&lala32,4);
		total = 0;
		do {
			bytes = send(sockfd, &buff[total], 7 - total,0);
			printf("B:%i\n",bytes);
			total += bytes;
		} while(bytes>0);
		bytes = send(sockfd, &buff[total],0,0);
		printf("Bytes enviados: %i\n",total);
	}
}

int main()
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	// function for chat
	func(sockfd);

	// close the socket
	close(sockfd);
}
