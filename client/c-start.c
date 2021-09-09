#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include "libs/game.h"
#include "libs/event_key.h"
#include "libs/eaeapp.h"

#define MAX_BUFFER_EVENTS 10

int main(char argc, char * argv[]){
	
	game_t *game;
	command_server_t *cs;
	req_key_press_t *buffer_events[MAX_BUFFER_EVENTS];
	int buffer_events_size;
	sem_t sem_buffer_events;
	pthread_t th_game;
	pthread_t th_command;

	/* Inicializamos el semaforo */
   sem_init(&sem_buffer_events,0,1);

	/* Inicializamos la estructura game */
	if (game_init(game,buffer_events,&sem_buffer_events,&buffer_events_size) < 0)
		return 1;

	/* Inicializamos la estructura de commandos */
	if(command_server_init(cs,buffer_events,&sem_buffer_events,&buffer_events_size))
		return 1;

	/* Creamos el hilo que se encarga del cliente de comandos */
	if(0 != pthread_create(&th_command, NULL, &cs_run, cs)){
		printf("Error al querer crear el hilo para el command\n");
		return 2;
	}

	/* Creamos el hilo que se encarga del juego en si mismo */
	if(0 != pthread_create(&th_game, NULL, &game_run, game)){
		printf("Error al querer crear el hilo para el juego\n");
		return 2;
	}

	pthread_join(th_game,NULL);
	pthread_join(th_command,NULL);

	/* Finalizamos el programa */
	game_free(game);
	return 0;
}
