#include <stdio.h>
#include "libs/point.h"
#include "libs/ship.h"
#include "libs/shoot.h"
#include "libs/game.h"
#include "../libs/tcp_server.h"
#include <sys/socket.h>
#include "libs/eaeapp_server.h"

int main(void *args){
	game_t game;
	tcp_server_t command_server;
	pthread_t th_command;
	pthread_t th_game;
	sem_t sem;

	uint8_t exit = 0;

	/* Inicializamos la estructura del juego */
	game_init(&game,&sem);
	printf("Paso 1\n");

	/* Inicializamos el server */
	if(!tcp_server_init(&command_server,2525)){
		printf("Error al querer generar el server");
		return 1;
	}
	printf("Paso 2\n");

	/* Asignamos el protocolo al server TCP */
	tcp_server_assign_protocol(&command_server,&server_protocol_handle);
	printf("Paso 3\n");

	/* Creamos el hilo que se encarga del servidor de comandos */
	if(0 != pthread_create(&th_command, NULL, &tcp_server_start, &command_server)){
		printf("Error al querer crear el hilo para el servidor de comandos\n");
		return 2;
	}
	printf("Paso 4\n");

	/* Creamos el hilo que se encarga de la logica del juego */
	if(0 != pthread_create(&th_game, NULL, &game_run, &game)){
		printf ("Imposible levantar el hilo para realizar tareas\n");
		return 2;
	}
	printf("Paso 5\n");
	pthread_join(th_command, NULL);
   pthread_join(th_game, NULL);
	printf("Fin\n");

	return 0;
}
