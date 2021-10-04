#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

// Function designed for chat between client and server.
void func(int sockfd) {
	char buff[MAX];
	int n, bytes, total;
	uint8_t lala;
	uint16_t lala16;
	uint32_t lala32;
	// infinite loop for chat
	for (;;) {
		bzero(buff, MAX);

		// read the message from client and copy it in buffer
		printf("Esperando datos\n");
		total = 0;
		bytes = recv(sockfd, buff, sizeof(buff),0);
		printf("Hemos recibido:%i\n",total);
		// print buffer which contains the client contents
		lala = buff[0];
		memcpy(&lala16,&(buff[1]),2);
		lala16 = ntohs(lala16);
		memcpy(&lala32,&(buff[3]),4);
		lala32 = ntohl(lala32);
		printf("Bytes: %i - Mensaje del cliente: %u, %u, %"PRIu32"\n",
			bytes, lala, lala16,lala32);
		bzero(buff, MAX);
		n = 0;
		// copy server message in the buffer
	}
}

// Driver function
int main()
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
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
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded.. %i\n",sockfd);

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("server accept failed...\n");
		exit(0);
	}
	else
		printf("server accept the client...\n");

	// Function for chatting between client and server
	func(connfd);

	// After chatting close the socket
	close(sockfd);
}
