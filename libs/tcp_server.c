#include "tcp_server"

void tcp_server_init(tcp_server_t *server, uint32_t port){
	server->fd_server
}

uint8_t tcp_server_start(tcp_server_t *s){
	/* Inicia el server. Retorna 0 si ocurre un error */

	/* Creamos el socket */
	if ((s->fd_server=socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {  
		printf("error al crear el socket()\n");
		return 0;
	}

	s->server.sin_family = AF_INET;
	s->server.sin_port = htons(port);
	s->server.sin_addr.s_addr = INADDR_ANY;

	/* Bindeamos el puerto e interfaz al soket */
	if(bind(s->fd_server,(struct sockaddr*)&(s->server), sizeof(struct sockaddr))<0) {
		printf("error en bind() \n");
		return 0;
	}

	/* Dejamos el socket en LISTEN */
	if(listen(s->fd_server,BACKLOG) == -1){ 
		printf("error en listen()\n");
		return 0;
	}
	return 1;
}

uint8_t tcp_server_recive(itcp_server_t *server){
	/* El server queda a la espera de una peticion por
	   parte del cliente */
}

uint8_t tcp_server_response(itcp_server_t *server){
	/* El server responde a un cliente */
}

void tcp_server_close(tcp_server_t *server){
	/* IMPLEMENTAR */
}
