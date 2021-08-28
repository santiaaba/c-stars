#include "tcp_server"

uint8_t tcp_server_init(tcp_server_t *server, uint32_t port){
	/* Creamos el socket */
	if ((server->fd_server=socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {  
		printf("error al crear el socket()\n");
		return 0;
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;

	/* Bindeamos el puerto e interfaz al soket */
	if(bind(server->fd_server,(struct sockaddr*)&(server->server), sizeof(struct sockaddr))<0) {
		printf("error en bind() \n");
		return 0;
	}

	/* Dejamos el socket en LISTEN */
	if(listen(server->fd_server,BACKLOG) == -1){ 
		printf("error en listen()\n");
		return 0;
	}
	server->running = 1;
	return 1;
}

void tcp_server_assign_protocol(tcp_server_t *server,
	void *protocol(char *req_buffer,int req_size, char *res_buffer, int *res_size)){
	/* Donde protocol es una funcion que maneja los mensajes que recibe el server */
	/* Puntualmente en nuestro caso es req_handle */
	server -> protocol = protocol;
}

void tcp_server_start(tcp_server_t *server){
	char *req_buffer;
	char *res_buffer;
	int req_size;
	int res_size;

	while(server->runing){
		/* Aguardamos que un cliente establezca una conexion */
		confd = accept(sockfd, (SA*)&cli, &len);
		if(confd > 0){
			server->status = ESTABLISHED;
			while(server->status == ESTABLISHED){
				/* Aguardamos a recibir una instruccion */
				size = read(server->sockfd, req_buffer, sizeof(req_buffer));
				server->protocol(req_buffer,req_size,&res_buffer,&res_size);
				/* Respondemos al cliente */
				if(size_response > 0)
					write(sever->sockfd,res_buffer,res_size);
			}
			close(server->sockfd);
		}
	}
}

void tcp_server_close(tcp_server_t *server){
	/* IMPLEMENTAR */
}
