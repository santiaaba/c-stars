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
	game_t game;
	tcp_server_t *command_server;
	udp_server_t *keyboard_server;

	pthread_t t_command;

	uint8_t exit = 0;

	/* Inicializamos la estructura del juego */
	game_init(&game);

	/* Creamos el hilo que se encarga del servidor de comandos */
	if(0 != pthread_create(&t_command, NULL, &command_server_start, s)){
		printf("Error al querer crear el hilo para el servidor de comandos\n");
		exit(2);
	}

	/* Creamos el hilo que se encarga de la logica del juego */
	if(0 != pthread_create(&(s->do_task), NULL, &game_run, &game)){
		printf ("Imposible levantar el hilo para realizar tareas\n");
		exit(2);
	}

	return 0;
}
