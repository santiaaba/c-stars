#include "tcp_server.h"

uint8_t tcp_server_init(tcp_server_t *server, uint32_t port, game_t *game){
//	void (*char2body)(char*, game_info_t*), void (*body2char)(char*, game_info_t*)){

	printf("Iniciamos el server\n");
	server->status = S_NOTLISTEN;
	server->game = game;
/*	server->status_char2body = char2body;
	server->status_body2char = body2char;
*/


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
		printf("Recibimos cierre de conexion\n");
		close(*confd);
		s->status = S_LISTEN;
	}

	s = (tcp_server_t*)server;
	len = sizeof(s->clientaddr);
	req_init(&req);
	res_init(&res);
	while(s->status == S_LISTEN){
		/* Aguardamos que un cliente establezca una conexion */
		printf("Aguardamos recibir una conexion\n");
		confd = accept(s->fd_server,(struct sockaddr*)&(s->clientaddr), &len);
		if(confd < 0){
			fprintf(stderr, "accept() failed: %s - %i\n",
				strerror(errno),confd);
			close(confd);
		} else{
			s->status = S_ESTABLISHED;
			printf("Cliente conectado\n");
			while(s->status == S_ESTABLISHED){
				/* Aguardamos a recibir un mensaje */
				printf("Esperando recibir un mensaje\n");
				printf("-------------------------------\n");
				//bzero(buffer, MAXBUFFER);
				bytes = recv(confd,buffer,MAXBUFFER,0);
				if(bytes <= 0){
					fprintf(stderr, "recv() header failed: %s\n",
						strerror(errno));
					tcp_server_close(s,&confd);
				} else {
					printf("bytes recibidos: %i\n",bytes);
					eaeapp_char2req(&req,buffer);
	
					/* Ya tenemos todos los datos. Ejecutamos el protocolo */
					server_protocol_handle(s->game, inet_ntoa(s->clientaddr.sin_addr),&req, &res);
					
					/* Los dos únicos mensajes provenientes del cliente que no
						esperan respuesta por parte del servidor. Son:
						C_KEY_PRESS y C_CONNECT_2 */
	
					if(res.header.cod != C_KEY_PRESS && res.header.cod != C_CONNECT_2){
						printf("Respuesta COD: %u\n",res.header.cod);
						/* Respondemos al cliente */
						eaeapp_res2char(&res,buffer,&size);
						printf("Enviando respuesta size : %i\n",size);
						bytes = send(confd,buffer,size,0);
						if(bytes < 0){
							printf("Error al enviar la respuesta\n");
							fprintf(stderr, "recv() failed: %s\n", strerror(errno));
						}
						printf("Bytes enviados: %i\n", bytes);
						printf("-------------------------------\n\n");
						if(req.header.cod == C_DISCONNECT)
							tcp_server_close(s,&confd);
					}
				}
			}  //While coneccion establecida
		}	//If accept
	}   //While listen
	printf("Se termino todo señores\n");
	close(s->fd_server);
}
