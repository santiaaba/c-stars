#include "tcp_client.h"

uint8_t tcp_client_init(tcp_client_t *cs, char *serv_ip, int port){
	cs->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (cs -> sockfd == -1){
		printf("Error al crear el socket TCP del cliente\n");
		return 0;
	}
	bzero(&(cs->servaddr),sizeof(cs->servaddr));

	// Asignacion de IP
	cs->servaddr.sin_family = AF_INET;
	cs->servaddr.sin_addr.s_addr = inet_addr(serv_ip);
	cs->servaddr.sin_port = htons(port);
	return 1;
}

uint8_t tcp_client_connect(tcp_client_t *cs){

	if(connect(cs->sockfd,(struct sockaddr*)&(cs->servaddr),
		sizeof(cs->servaddr)) != 0){
		printf("Error de conexion contra el servidor\n");
		return 0;
	}
	return 1;
}

void cs_close(tcp_client_t *cs){
	close(cs->sockfd);
}

void tcp_client_send(tcp_client_t *cs, char *req, int req_size, void handle_res(char*,int)){
	/* Se encarga de recibir un mensaje a aser enviado y la funcion que se encarga
	   de prosesar la respuesta. Si dicha función es NULL entonces no es necesario
	   procesar el mensaje obtenido */
	char res_buffer[MAX_RES_BUFFER];
	int buffer_size;

	memcpy(&res_buffer,req,req_size);
	res_buffer[req_size + 1] = '\n';
	printf("Enviando al cliente. SIZE: %i, BUFFER: %s\n",req_size,req);
	send(cs->sockfd,req,req_size,0);
	if(handle_res != NULL){
		bzero(res_buffer, MAX_RES_BUFFER);
		printf("Esperando respuesta del server\n");
		buffer_size = recv(cs->sockfd,res_buffer,MAX_RES_BUFFER,0);
		printf("Recivido del servidor\n");
		handle_res(res_buffer,buffer_size);
	}
}
