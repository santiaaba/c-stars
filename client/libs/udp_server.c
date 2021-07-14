#include "server_udp.h"

int server_udp_init(server_udp_t *server){

	server->status = 1;
	// Creating socket file descriptor
	if ( (server -> sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	  
	memset(&(server->servaddr), 0, sizeof(server->servaddr));
	memset(&(server->cliaddr), 0, sizeof(server->cliaddr));
	  
	// Filling server information
	server->servaddr.sin_family	= AF_INET; // IPv4
	server->servaddr.sin_addr.s_addr = INADDR_ANY;
	server->servaddr.sin_port = htons(PORT);
	  
	// Bind the socket with the server address
	if ( bind(server->sockfd, (const struct sockaddr *)&(server->servaddr), 
			sizeof(server->servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	  
}

int server_udp_start(server_udp_t *server, screen_t *screen){
	int len, n;
  
	len = sizeof(server->cliaddr);  //len is value/resuslt
  
	while(server->status == 1){
		n = recvfrom(server->sockfd, (char *)buffer, MAXLINE, 
			 			MSG_WAITALL, ( struct sockaddr *) &(server->cliaddr),
		 				&len);

		/* Por cada udp recibido */
		/* Obtenemos el encabezado */
		if(frame > screen->last_frame){
			/* Renderizamos la pantalla */
			/* El screen_draw vacia el buffer */
			screen_draw(screen);
		}
		/* Poblamos el buffer con las entidades */
		for(algo){
			screen_add_entity(screen,entity);
		}
	}

	return 0;
}
