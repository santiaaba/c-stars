#include "tcp_server.h"

uint8_t tcp_server_init(tcp_server_t *server, uint32_t port, game_t *game,
	void (*char2body)(char*, game_info_t*), void (*body2char)(char*, game_info_t*)){

	printf("Iniciamos el server\n");
	server->status = S_NOTLISTEN;
	server->game = game;
	server->status_char2body = char2body;
	server->status_body2char = body2char;


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
	printf("Bindeamos server\n");
	if(bind(server->fd_server,(struct sockaddr*)&(server->serveraddr),
		sizeof(struct sockaddr)) != 0) {
			fprintf(stderr, "recv() failed: %s\n", strerror(errno));
			return 0;
	}

	/* Dejamos el socket en LISTEN */
	printf("Dejamos socket en LISTEN\n");
	if(listen(server->fd_server,1) != 0){ 
		fprintf(stderr, "recv() failed: %s\n", strerror(errno));
		return 0;
	}
	server->status = S_LISTEN;
	printf("Servidor listo para recibir una conexion\n");
	return 1;
}

void *tcp_server_start(void *server){
	char buffer[MAXBUFFER];
	int size, bytes;
	int res_size;
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
			fprintf(stderr, "accept() failed: %s - %i\n", strerror(errno),confd);
			close(confd);
		} else{
			s->status = S_ESTABLISHED;
			printf("Cliente conectado\n");
			while(s->status == S_ESTABLISHED){
				/* Aguardamos a recibir un mensaje */
				printf("Esperando recibir un encabezado\n");
				bzero(buffer, MAXBUFFER);
				bytes = recv(confd,buffer, REQ_HEADER_SIZE,0);
				if(bytes < 0){
					fprintf(stderr, "recv() header failed: %s\n",
						strerror(errno));
					tcp_server_close(s,&confd);
				}
				printf("bytes recibidos: %i\n",bytes);
				memcpy(&size,&(buffer[2]),2);
				size = ntohs(size);
				if(size > 0){
					printf("Esperando recibir el body. Bytes esperados: %i\n",
						size);
					bytes = recv(confd,&(buffer[REQ_HEADER_SIZE]),size,0);
					printf("bytes recibidos: %i\n",bytes);
					if(bytes < 0){
						fprintf(stderr, "recv() body failed: %s\n",
							strerror(errno));
						tcp_server_close(s,&confd);
					}
				}
				eaeapp_char2req(&req,buffer);

				printf("Procesamos el pedido\n");
				if(res.body != NULL)
					free(res.body);
				/* Ya tenemos todos los datos. Ejecutamos el protocolo */
				server_protocol_handle(s->game, &req, &res);
				
				printf("Respuesta COD: %u\n",res.header.cod);
				/* Respondemos al cliente */
				printf("Enviamos la respuesta\n");
				eaeapp_res2char(&res,buffer,&size);

				/* Enviamos el encabezado */
				printf("Enviamos el encabezado de la respuesta\n");
				bytes = send(confd,buffer,RES_HEADER_SIZE,0);
				if(bytes < 0){
					printf("Error al enviar el encabezado de la respuesta\n");
					fprintf(stderr, "recv() failed: %s\n", strerror(errno));
				}
				printf("Bytes enviados: %i\n", bytes);

				/* Enviamos la cuerpo */
				printf("Enviamos el body de la respuesta\n");
				bytes = send(confd,&(buffer[RES_HEADER_SIZE]),res.header.size,0);
				printf("Bytes enviados: %i\n", bytes);
				if(bytes < 0){
					printf("Error al enviar el body de la respuesta\n");
					fprintf(stderr, "recv() failed: %s\n", strerror(errno));
				}

				if(req.header.cod == C_DISCONNECT)
					tcp_server_close(s,&confd);
			}  //While coneccion establecida
		}	//If accept
	}   //While listen
	close(s->fd_server);
	printf("Se termino todo señores\n");
}
