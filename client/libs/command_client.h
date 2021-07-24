#ifndef COMMAND_CLIENT_H
#define COMMAND_CLIENT_H

#include <sys/socket.h>

#define PORT 25252

typedef struct {
	int sockfd;
   struct sockaddr_in servaddr;
	char buffer[100];
} command_server_t;

void command_server_init(command_server_t *cs);
void command_server_connect(command_server_t *cs);
void command_server_close(command_server_t *cs);

#ifndef
