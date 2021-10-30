#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include "libs/game.h"

int main(char argc, char * argv[]){
	
	game_t *game;

	game = (game_t*)malloc(sizeof(game_t));
	/* Inicializamos la estructura game */
	if (!game_init(game))
		return 55;

	game_run(game);

	/* Finalizamos el programa */
	game_free(game);
	return 0;
}
