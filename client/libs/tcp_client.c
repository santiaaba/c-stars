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

int tcp_client_send(tcp_client_t *cs, req_t *req, void handle_res(res_t*)){
	/* Se encarga de recibir un mensaje a aser enviado y la funcion que se encarga
	   de prosesar la respuesta. Si dicha función es NULL entonces no es necesario
	   procesar el mensaje obtenido como respuesta. Retorna -1 si ha habido
		algun error */
	char buffer[MAX_RES_BUFFER];
	int size, total, bytes;
	res_t res;

	/* Enviamos el encabezado de req */
	bzero(buffer, MAX_RES_BUFFER);
	eaeapp_req_header2char(req,buffer,&size);
	bytes = send(cs->sockfd,buffer,size,0);
	if(bytes < 0){
		printf("Error al enviar el encabezado al server\n");
		return -1;
	}

	/* Enviamos el cuerpo del req */
	eaeapp_req_body2char(req,buffer,&size);
	total = 0;
	while(total < size){
		bytes = send(cs->sockfd,&buffer[total],req->header.size - total,0);
		if(bytes > 0)
			total += bytes;
		else
			if(bytes < 0){
				printf("ERROR al envir la consulta\n");
				return -1;
			} else {
				printf("Socket se ha cerrado\n");
				return -1;
			}
	}

	/* OJO!!!! Quizas nos convega siempre enviar
	   desde el server la respuesta. Si llegamos a
		querer enviar una respueta desde el server
		pero en el cliente no se espera la misma, el
		server queda colgado tratando de realizar el
		envío */
	if(handle_res == NULL)
		return 0;
	
	/* Recibimos el enabezado de res */
	bzero(buffer, MAX_RES_BUFFER);
	printf("Esperando encabezado de la respuesta del server\n");
	bytes = recv(cs->sockfd,buffer,MAX_RES_BUFFER,0);
	if(bytes < 0){
		printf("Error al recibir el encabezado de la respuesta\n");
		return -1;
	}
	eaeapp_res_char2header(&res,buffer,bytes);

	/* Recibimos el cuerpo de res */
	printf("Esperando body de la respuesta del server\n");
	total = 0;
	while(total < res.header.size){
		bytes = recv(cs->sockfd,&buffer[total],MAX_RES_BUFFER,0);
		if(bytes > 0)
			total += bytes;
		else
			if(bytes < 0){
				printf("ERROR al envir la consulta\n");
				return -1;
			} else {
				printf("Socket se ha cerrado\n");
				return -1;
			}
	}
	eaeapp_res_char2body(&res,buffer,total,FALTA_LA_FUNCION);
	handle_res(&res);
	return 0;
}

