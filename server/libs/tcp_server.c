#include "tcp_server.h"

uint8_t tcp_server_init(tcp_server_t *server, uint32_t port, game_t *game){
	printf("Iniciamos el server\n");
	server->status = S_NOTLISTEN;
	server->game = game;
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
	if(listen(server->fd_server,5) != 0){ 
		printf("error en listen()\n");
		return 0;
	}
	server->status = S_LISTEN;
	printf("Servidor listo para recibir una conexion\n");
	return 1;
}

void tcp_server_assign_protocol(tcp_server_t *server,
      void (*protocol)(void*, char*, int, char*, int*)){
	server->protocol = protocol;
	printf("Protocolo asignado\n");
}

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
			printf("Cliente connected\n");
			while(s->status == S_ESTABLISHED){

				/* Aguardamos a recibir un encabezado */
				printf("Esperando recibir un encabezado del cliente\n");
				size = recv(s->fd_server, req_buffer , REQ_HEADER_SIZE , 0);
				if(size < 0){
					printf("Ha ocurrido un error fatal al recibir el encabezado\n");
				} else {
					if(size != REQ_HEADER_SIZE){
						printf("El encabezado no respeta el tamano esperado\n")
					}
				}
				eaeapp_req_char2header(&req, req_buffer, size);

				/* Pasamos a recibir el body */
				total = 0;
				while(total < req->header.size){
					bytes = recv(s->fd_server, req_buffer[body_received] , req->header.size , 0);
					if(bytes > 0)
						total += bytes;
					else
						if(bytes < 0)
							printf("ERROR al recibir el encabezado\n");
						else
							/* Se ha cerrado el socket */
							printf("Socket cerrado????\n");
				}
				eaeapp_req_char2body(&req, req_buffer, total);

				/* Ya tenemos todos los datos. Ejecutamos el protocolo */
				server_protocol_handle(game, &req, &res)
				
				/* Respondemos al cliente si cod es distinto de 0 */
				if(res.cod != 0){
					/* Enviamos el encabezado */
					eaeapp_res_header2char(res, &buffer, &size);
					send(s->fd_server,buffer,size,0);
					/* Enviamos el body */
					eaeapp_res_body2char(res,&buffer,&size);
					total = 0;
					while(total < res.size){
						bytes = send(s->fd_server,buffer,size,0);
						if(bytes > 0)
							total += size
						else
							if(bytes < 0)
								printf("ERROR al envir la respuesta\n");
							else
								printf("Socket se ha cerrado\n");
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
