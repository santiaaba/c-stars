#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../../libs/eaeapp.h"
#include "../../libs/game_info.h"
#include "eaeapp_server.h"
#include "game.h"
#include <errno.h>

#define	S_NOTLISTEN			0
#define	S_LISTEN				1
#define	S_ESTABLISHED		2

#define MAXBUFFER				512
#define REQ_HEADER_SIZE		8

typedef struct{
	int fd_server;
	int running;
	int status;
	game_t *game;
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	void (*status_char2body)(char*, game_info_t*);
	void (*status_body2char)(char*, game_info_t*);
} tcp_server_t;

uint8_t tcp_server_init(tcp_server_t *server, uint32_t port, game_t *game);
//			void (*char2body)(char*, game_info_t*), void (*body2char)(char*, game_info_t*));

void *tcp_server_start(void *server);

void tcp_server_close(tcp_server_t *server);

#endif

