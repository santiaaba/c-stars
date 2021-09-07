#include "libs/point.h"
#include "libs/ship.h"
#include "libs/shoot.h"
#include "libs/game.h"
#include "../libs/tcp_server.h"
#include <sys/socket.h>
#include "libs/command_protocol.h"

void main(void *args){
	game_t game;
	tcp_server_t command_server;
	pthread_t thread_command;
	pthread_t thread_game;
	sem_t sem;

	uint8_t exit = 0;

	/* Inicializamos la estructura del juego */
	game_init(&game,&sem);

	/* Inicializamos el server */
	if(!tcp_server_init(&command_server,2525)){
		printf("Error al querer generar el server");
		return 1;
	}

	/* Asignamos el protocolo al server TCP */
	tcp_server_assign_protocol(&command_server,&req_handle);

	/* Creamos el hilo que se encarga del servidor de comandos */
	if(0 != pthread_create(&thread_command, NULL, &tcp_server_start, &command_server)){
		printf("Error al querer crear el hilo para el servidor de comandos\n");
		exit(2);
	}

	/* Creamos el hilo que se encarga de la logica del juego */
	if(0 != pthread_create(&thread_game, NULL, &game_run, &game)){
		printf ("Imposible levantar el hilo para realizar tareas\n");
		exit(2);
	}

	return 0;
}
