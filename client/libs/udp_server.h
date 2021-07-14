#ifndef SERVER_UDP_H
#define SERVER_UDP_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

typedef struct {
	int sockfd;
	struct sockaddr_in servaddr, cliaddr;
	int status;
} server_udp_t;

int server_udp_init(server_udp_t *server);
int server_udp_start(server_udp_t *server, screen_t *screen);

#endif
