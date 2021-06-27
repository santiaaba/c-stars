#include "libs/point.h"
#include "libs/ship.h"
#include "libs/shoot.h"
#include "libs/game.h"
#include <sys/socket.h>
#include "command_server.h"

void command_server_start(){
}

void server_init(){

	/* Creamos el hilo que se encarga del servidor de comandos */
	if(0 != pthread_create(&(s->thread), NULL, &command_server_start, s)){
		exit(2);
	}
	/* Creamos el hilo que se encarga de la logica del juego  */
	if(0 != pthread_create(&(s->do_task), NULL, &server_do_task, s)){
		printf ("Imposible levantar el hilo para realizar tareas\n");
		exit(2);
	}
}

void main(void *args){
	game_t *game;
	tcp_server_t *command_server;
	udp_server_t *keyboard_server;
	uint8_t exit = 0;

	while(!exit){
		/* Levantamos el puerto de comandos */
		/* Bindeamos el socket */
		/* Aguardamos una conexión */

		/* Negociamos los puertos del render, comandos y datos */

		game = game_init()

		/* Pasamos los socket al juego */
		game_bind(game,fd_render,fd_command,fd_data)

		/* Iniciamos el juego */
		game_start(game)
	}
	return 0;
}
