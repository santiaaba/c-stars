#include "libs/point.h"
#include "libs/ship.h"
#include "libs/shoot.h"
#include "libs/game.h"
#include <sys/socket.h>
#include "command_server.h"

void main(void *args){
	game_t game;
	tcp_server_t command_server;
	pthread_t thread_command;
	pthread_t thread_game;

	uint8_t exit = 0;

	/* Inicializamos la estructura del juego */
	game_init(&game);

	/* Inicializamos el server */
	if(!tcp_server_init(command_server,2525)){
		printf("Error al querer generar els erver")
		return 1;
	}

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
