#include <stdio.h>
#include "libs/point.h"
#include "libs/ship.h"
#include "libs/shoot.h"
#include "libs/game.h"
#include <pthread.h>
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

	/* Inicializamos el semaforo */
	sem_init(&sem,0,1);

	/* Inicializamos la estructura del juego */
	game_init(&game,&sem);

	/* Creamos el hilo que se encarga de la logica del juego */
	printf("Hilo para el jugador\n");
	if(0 != pthread_create(&th_game, NULL, &game_run, &game)){
		printf ("Imposible levantar el hilo para realizar tareas\n");
		return 2;
	}

	/* Inicializamos el server */
	printf("Inicializamos server tcp\n");
	if(!tcp_server_init(&command_server,2525)){
		printf("Error al querer generar el server");
		return 1;
	}

	/* Asignamos el protocolo al server TCP */
	printf("Asignamos protocolo\n");
	tcp_server_assign_protocol(&command_server,&server_protocol_handle);

	/* Creamos el hilo que se encarga del servidor de comandos */
	printf("Hilo para el server\n");
	if(0 != pthread_create(&th_command, NULL, &tcp_server_start, &command_server)){
		printf("Error al querer crear el hilo para el servidor de comandos\n");
		return 2;
	}

	pthread_join(th_command, NULL);
   pthread_join(th_game, NULL);
	printf("Fin\n");

	return 0;
}
