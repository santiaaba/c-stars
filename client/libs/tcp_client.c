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

void tcp_client_disconnect(tcp_client_t *cs){
	close(cs->sockfd);
}

int tcp_client_send(tcp_client_t *cs, req_t *req, void handle_res(res_t*)){
	/* Se encarga de recibir un mensaje a ser enviado y la funcion que se encarga
	   de prosesar la respuesta. Si dicha función es NULL entonces no es necesario
	   procesar el mensaje obtenido como respuesta. Retorna -1 si ha habido
		algun error */
	char buffer[MAX_BUFFER];
	int size, bytes;
	res_t res;

	res_init(&res);
	bzero(buffer, MAX_BUFFER);
	eaeapp_req2char(req,buffer,&size);

	/* Enviamos el mensaje */
	printf("----------------------------\n");
	bytes = send(cs->sockfd,&buffer,size,0);
	printf("Bytes enviados: %i\n",bytes);
	if(bytes < 0){
		printf("Error al enviar la petición al server\n");
		return -1;
	}
	/* Recibimos la respuesta */
	if(handle_res != NULL){
		bytes = recv(cs->sockfd,buffer,MAX_BUFFER,0);
		printf("bytes recibidos(MAX_BUFFER=%i): %i\n",MAX_BUFFER,bytes);
		eaeapp_char2res(&res,buffer);
		printf("Paso el eaeapp\n");
		handle_res(&res);
	}
	printf("----------------------------\n\n");
	return 0;
}

