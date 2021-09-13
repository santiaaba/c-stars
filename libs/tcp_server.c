#include "tcp_server.h"

uint8_t tcp_server_init(tcp_server_t *server, uint32_t port){
	printf("Iniciamos el server\n");
	server->status = S_NOTLISTEN;
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
	char req_buffer[MAXBUFFER];
	char res_buffer[MAXBUFFER];
	int req_size;
	int res_size;
	int confd, len;
	tcp_server_t *s;

	s = (tcp_server_t*)server;
	while(s->status == S_LISTEN){
		/* Aguardamos que un cliente establezca una conexion */
		confd = accept(s->fd_server, (struct sockaddr*)&s->clientaddr, &len);
		if(confd > 0){
			s->status = S_ESTABLISHED;
			while(s->status == S_ESTABLISHED){
				/* Aguardamos a recibir una instruccion */

				//read(server->fd_server, req_buffer, sizeof(req_buffer));
				req_size = recv(s->fd_server, req_buffer , MAXBUFFER , 0);
				if(req_size > 0){
					(s->protocol)(req_buffer,req_size,res_buffer,&res_size);
					/* Respondemos al cliente */
					if(res_size > 0)
						send(s->fd_server,res_buffer,res_size,0);
				}
			}
			close(s->fd_server);
		}
	}
}

void tcp_server_close(tcp_server_t *server){
	/* IMPLEMENTAR */
}
