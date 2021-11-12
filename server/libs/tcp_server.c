#include "tcp_server.h"

uint8_t tcp_server_init(tcp_server_t *server, uint32_t port, game_t *game){
	server->status = S_NOTLISTEN;
	server->game = game;

	/* Creamos el socket */
	server->fd_server=socket(AF_INET, SOCK_STREAM, 0);
	if(server->fd_server == -1){
		fprintf(stderr, "recv() failed: %s\n", strerror(errno));
		return 0;
	}

	server->serveraddr.sin_family = AF_INET;
	server->serveraddr.sin_port = htons(port);
	server->serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	/* Bindeamos el puerto e interfaz al soket */
	if(bind(server->fd_server,(struct sockaddr*)&(server->serveraddr),
		sizeof(struct sockaddr)) != 0) {
			fprintf(stderr, "recv() failed: %s\n", strerror(errno));
			return 0;
	}

	/* Dejamos el socket en LISTEN */
	if(listen(server->fd_server,1) != 0){ 
		fprintf(stderr, "recv() failed: %s\n", strerror(errno));
		return 0;
	}
	server->status = S_LISTEN;
	return 1;
}

void *tcp_server_start(void *server){
	char buffer[MAXBUFFER];
	int size, bytes_recv, bytes_resp;
	int res_size;
	int aux;
	int confd, len;
	tcp_server_t *s;
	req_t req;
	res_t res;

	void tcp_server_close(tcp_server_t *s, int *confd){
		close(*confd);
		s->status = S_LISTEN;
	}

	s = (tcp_server_t*)server;
	len = sizeof(s->clientaddr);
	req_init(&req);
	res_init(&res);
	while(s->status == S_LISTEN){
		/* Aguardamos que un cliente establezca una conexion */
		confd = accept(s->fd_server,(struct sockaddr*)&(s->clientaddr), &len);
		if(confd < 0){
			fprintf(stderr, "accept() failed: %s - %i\n",
				strerror(errno),confd);
			close(confd);
		} else{
			s->status = S_ESTABLISHED;
			while(s->status == S_ESTABLISHED){
				/* Aguardamos a recibir un mensaje */
				bytes_recv = recv(confd,buffer,MAXBUFFER,0);
				if(bytes_recv <= 0){
					fprintf(stderr, "recv() header failed: %s\n",
						strerror(errno));
					tcp_server_close(s,&confd);
				} else {
					/* Puede que tengamos más de un mensaje entre los bytes recibidos */
					aux = 0;
					while(aux != bytes_recv){
						aux += eaeapp_char2req(&req,&buffer[aux]);
						/* Ya tenemos todos los datos. Ejecutamos el protocolo */
						server_protocol_handle(s->game, inet_ntoa(s->clientaddr.sin_addr),&req, &res);
						/* Los dos únicos mensajes provenientes del cliente que no
							esperan respuesta por parte del servidor. Son:
							C_KEY_PRESS y C_CONNECT_2 */
						if(res.header.cod != C_KEY_PRESS && res.header.cod != C_CONNECT_2){
							/* Respondemos al cliente */
							eaeapp_res2char(&res,buffer,&size);
							bytes_resp = send(confd,buffer,size,0);
							if(bytes_resp < 0){
								fprintf(stderr, "recv() failed: %s\n", strerror(errno));
							}
							if(req.header.cod == C_DISCONNECT)
								tcp_server_close(s,&confd);
						}
					}
				}
			}  //While coneccion establecida
		}	//If accept
	}   //While listen
	printf("Se termino todo señores\n");
	close(s->fd_server);
}
