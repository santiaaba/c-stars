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

void tcp_server_start(tcp_server_t *server){

	while(server->runing){
		/* Aguardamos que un cliente establezca una conexion */
		confd = accept(sockfd, (SA*)&cli, &len);
		if(confd > 0){
			server->status = ESTABLISHED;
			while(server->status == ESTABLISHED){
				/* Aguardamos a recibir una instruccion */
				size = read(server->sockfd, buff, sizeof(buff));
				server->protocol(buffer,size);
				/* Respondemos al cliente */
				write(sever->sockfd, buff, sizeof(buff));
			}
			close(server->sockfd);
		}
	}
}

void tcp_server_close(tcp_server_t *server){
	/* IMPLEMENTAR */
}
