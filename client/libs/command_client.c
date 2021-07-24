#include "command_client.h"

void cs_init(command_server_t *cs){
	
}

void cs_connect(command_server_t *cs, char *srv_ip){
	cs-> sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (cs -> sockfd == -1){
		printf("Error al crear el socket TCP\n");
		return 0;
	}
	bzero(&(cs->servaddr),sizeof(cs->servaddr));

	// Asignacion de IP
	cs->servaddr.sin_family = AF_INET;
	cs->servaddr.sin_addr.s_addr = inet_addr(serv_ip);
	cs->servaddr.sin_port = htons(PORT);

	if(connect(sockfd,(struct sockaddr*)&(cs->servaddr),
		sizeof(cs->servaddr)) != 0){
		printf("Error de conexion contra el servidor\n");
		return 0;
	}
	return 1;
}

void cs_close(command_server_t *cs){
	close(cs->sockfd);
}

/* Verifica continuamente si en el buffer hay datos
   a transmitir y los envia */
void cs_run(command_server_t *cs){
	char *buffer_aux[100];
	int buffer_size;

	while(true){
		if(strlen(cs->buffer_size) > 0){
			/* Liberamos el buffer para que si se
				presiona una nueva tecla no quede atascado
				a causa del semaforo */
			memcpy(buffer_aux,cs->buffer,100);
			buffer_size = strlen(buffer_aux);
			while(buffer_size){
				write(cs->sockfd, cs->buffer, sizeof(cs->buffer));
				bzero(cs->buffer, sizeof(cs->buffer));
				read(cs->sockfd, cs->buffer, sizeof(cs->buffer));
				buffer_size = 0;
			}
		}
	}
}
