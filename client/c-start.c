#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include "libs/game.h"

int main(char argc, char * argv[]){
	
	game_t *game;
	pthread_t thread_game;

	if (game_init(game) < 0){
		exit(1);
	}

	/* Creamos el hilo que se encarga del juego en si mismo */
	if(0 != pthread_create(&thread_game, NULL, &game_run, game)){
		printf("Error al querer crear el hilo para el juego\n");
		exit(2);
	}

	/* Finalizamos el programa */
	game_free(game);
	return 0;
}
