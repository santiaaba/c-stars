#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

typedef {
	uint8_t cod,
	uint8_t aux,
	uint16_t size,
	uint32_t qid
} mensaje_t

int fd_server;
int fd_client;
struct sockaddr_in server;
struct sockaddr_in client;

void main(){

	if ((fd_server=socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {
      printf("error al crear el socket()\n");
      return 0;
   }

	sin_family = AF_INET;
   sin_port = htons(port);
   sin_addr.s_addr = INADDR_ANY;

   /* Bindeamos el puerto e interfaz al soket */
   if(bind(fd_server,(struct sockaddr*)&(server), sizeof(struct sockaddr))<0) {
      printf("error en bind() \n");
      return 0;
   }

   /* Dejamos el socket en LISTEN */
   if(listen(fd_server,BACKLOG) == -1){
      printf("error en listen()\n");
      return 0;
   }

}
