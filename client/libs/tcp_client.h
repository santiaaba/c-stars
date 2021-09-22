#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../../libs/eaeapp.h"

#define MAX_RES_BUFFER	512

typedef struct {
	int sockfd;
   struct sockaddr_in servaddr;
	char buffer[100];
} tcp_client_t;

uint8_t tcp_client_init(tcp_client_t *cs, char *serv_ip, int port);
uint8_t tcp_client_connect(tcp_client_t *cs);
int tcp_client_send(tcp_client_t *cs, req_t *req, void handle_res(res_t*));
void cs_close(tcp_client_t *cs);


#endif
