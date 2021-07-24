#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include "libs/game.h"

int main(char argc, char * argv[]){
	
	game_t *game;
	command_server_t *cs

	pthread_t thread_game;
	pthread_t thread_cs;
	void *retval;

	/* Inicializamos la estructura game */
	if (game_init(game) < 0)
		exit(1);

	/* Inicializamos la estructura de commandos */
	if(command_server_init(cs))
		exit(1);

	/* Creamos el hilo que se encarga del cliente de comandos */
	if(0 != pthread_create(&thread_cs, NULL, &cs_run, cs)){
		printf("Error al querer crear el hilo para el command\n");
		exit(2);
	}

	/* Creamos el hilo que se encarga del juego en si mismo */
	if(0 != pthread_create(&thread_game, NULL, &game_run, game)){
		printf("Error al querer crear el hilo para el juego\n");
		exit(2);
	}

	pthread_join(thread_game,&retval);

	/* Finalizamos el programa */
	game_free(game);
	return 0;
}
