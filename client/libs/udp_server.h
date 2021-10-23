#ifndef UDP_SERVER_H
#define UDP_SERVER_H

typedef struct {
	int sockfd;
	struct sockeaddr_in cliaddr;
	struct sockeaddr_in servaddr;
	data_t *buffer_render;		  // Buffer render
	int buffer_render_size;
} udp_server_t

int udp_server_init(udp_server_t *s);
#endif
