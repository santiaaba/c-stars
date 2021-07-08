#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <stdlib.h>
#include <stdint.h>
#include <soket.h>

typedef enum{
	LISTEN
	STABLISHED
} server_status_t;

typedef struct t_tcp_server{
	int fd_server;
	int fd_client;
	int running;
	server_status_t status;
	struct sockaddr_in server;
	struct sockaddr_in client;
} tcp_server_t

void tcp_server_init(tcp_server_t *server, uint32_t port);

void tcp_server_assign_protocol(
			tcp_server_t *server,
			void *protocol(char *buffer,int size)
		);

uint8_t tcp_server_start(tcp_server_t *server);

uint8_t tcp_server_recive(tcp_server_t *server);

uint8_t tcp_server_response(tcp_server_t *server);

void tcp_server_close(tcp_server_t *server);

#endif

