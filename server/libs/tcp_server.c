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
		printf("error al crear el socket()\n");
		return 0;
	}

	server->serveraddr.sin_family = AF_INET;
	server->serveraddr.sin_port = htons(port);
	server->serveraddr.sin_addr.s_addr = INADDR_ANY;

	/* Bindeamos el puerto e interfaz al soket */
	printf("Bindeamos server\n");
	if(bind(server->fd_server,(struct sockaddr*)&(server->serveraddr),
		sizeof(struct sockaddr))<0) {
			printf("error en bind() \n");
			return 0;
	}

	/* Dejamos el socket en LISTEN */
	printf("Dejamos socket en LISTEN\n");
	if(listen(server->fd_server,1) != 0){ 
		printf("error en listen()\n");
		return 0;
	}
	server->status = S_LISTEN;
	printf("Servidor listo para recibir una conexion\n");
	return 1;
}

/*
void tcp_server_assign_protocol(tcp_server_t *server,
      void (*protocol)(void*, char*, int, char*, int*)){
	server->protocol = protocol;
	printf("Protocolo asignado\n");
}
*/

void *tcp_server_start(void *server){
	char buffer[MAXBUFFER];
	int size, total, bytes;
	int res_size;
	int confd, len;
	tcp_server_t *s;
	req_t req;
	res_t res;

	s = (tcp_server_t*)server;
	while(s->status == S_LISTEN){
		/* Aguardamos que un cliente establezca una conexion */
		confd = accept(s->fd_server, (struct sockaddr*)&s->clientaddr, &len);
		if(confd > 0){
			s->status = S_ESTABLISHED;
			printf("Cliente conectado\n");
			while(s->status == S_ESTABLISHED){

				/* Aguardamos a recibir un mensaje */
				printf("Esperando recibir un mensaje\n");
				/* Cuando bytes sea 0 significa que ya hemos recibido el total de los datos */
				total = 0;
				while (bytes = recv(s->fd_server, &(buffer[total]), MAXBUFFER ,0) > 0){
					/* Vamos recibiendo los datos */
					total += bytes;
				}
				sleep(10);
				if(bytes < 0){
					printf("Ha ocurrido un error fatal al recibir los datos\n");
					continue;
				}
				if(total > MAXBUFFER){
					printf("Overfflow de Buffer. Descartamos el mensaje\n");
					continue;
				}
				eaeapp_char2req(&req,buffer);
				/* Ya tenemos todos los datos. Ejecutamos el protocolo */
				server_protocol_handle(s->game, &req, &res);
				
				/* Respondemos al cliente si cod es distinto de 0 */
				if(res.header.cod != 0){
					eaeapp_res2char(&res,buffer,&size);
					/* Enviamos la respuesta */
					total = 0;
					while(bytes = send(s->fd_server,&(buffer[total]),size-total,0) > 0){
						total += bytes;
					}
					if(bytes < 0){
						printf("Error al enviar la respuesta\n");
					}
				}
			}
			close(s->fd_server);
		}
	}
}

void tcp_server_close(tcp_server_t *server){
	/* IMPLEMENTAR */
}
