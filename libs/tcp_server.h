#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define	S_NOTLISTEN			0
#define	S_LISTEN				1
#define	S_ESTABLISHED		2

#define MAXBUFFER				512

typedef struct{
	int fd_server;
	int fd_client;
	int running;
	int status;
	void (*protocol)(char *req_buffer,int req_size, char *res_buffer, int *res_size);
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
} tcp_server_t;

uint8_t tcp_server_init(tcp_server_t *server, uint32_t port);

void tcp_server_assign_protocol(tcp_server_t *server,
		void (*protocol)(void*, char*, int, char*, int*));

void *tcp_server_start(void *server);

void tcp_server_close(tcp_server_t *server);

#endif

