#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <stdlib.h>
#include <stdint.h>
#include <soket.h>

typedef struct t_tcp_server{
	int fd_server;
	int fd_client;
	struct sockaddr_in server;
	struct sockaddr_in client;
} tcp_server_t

void tcp_server_init(tcp_server_t *server, uint32_t port);

uint8_t tcp_server_start(tcp_server_t *server);

uint8_t tcp_server_recive(itcp_server_t *server);

uint8_t tcp_server_response(itcp_server_t *server);

void tcp_server_close(tcp_server_t *server);

#endif

