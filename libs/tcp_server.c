#include "tcp_server.h"

uint8_t tcp_server_init(tcp_server_t *server, uint32_t port){
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
	if(bind(server->fd_server,(struct sockaddr*)&(server->serveraddr),
		sizeof(struct sockaddr))<0) {
			printf("error en bind() \n");
			return 0;
	}

	/* Dejamos el socket en LISTEN */
	if(listen(server->fd_server,5) != 0){ 
		printf("error en listen()\n");
		return 0;
	}
	server->status = S_LISTEN;
	return 1;
}

void tcp_server_assign_protocol(tcp_server_t *server,
      void (*protocol)(char*, int, char*, int)){
	server->protocol = protocol;
}

void tcp_server_start(tcp_server_t *server){
	char req_buffer[MAXBUFFER];
	char res_buffer[MAXBUFFER];
	int req_size;
	int res_size;
	int confd, len;

	while(server->status == S_LISTEN){
		/* Aguardamos que un cliente establezca una conexion */
		confd = accept(server->fd_server, (struct sockaddr*)&server->clientaddr, &len);
		if(confd > 0){
			server->status = S_ESTABLISHED;
			while(server->status == S_ESTABLISHED){
				/* Aguardamos a recibir una instruccion */

				//read(server->fd_server, req_buffer, sizeof(req_buffer));
				req_size = recv(server->fd_server, req_buffer , MAXBUFFER , 0);
				if(req_size > 0){
					(server->protocol)(req_buffer,req_size,res_buffer,&res_size);
					/* Respondemos al cliente */
					if(res_size > 0)
						send(server->fd_server,res_buffer,res_size,0);
				}
			}
			close(server->fd_server);
		}
	}
}

void tcp_server_close(tcp_server_t *server){
	/* IMPLEMENTAR */
}
