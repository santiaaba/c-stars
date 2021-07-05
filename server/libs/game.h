#ifndef GAME_H
#define GAME_H

#include "point.h"
#include "ship.h"
#include "shoot.h"
#include <stdint.h>

typedef enum {
	WAIT,		/* Esperando a que se conecte un cliente */
	READY,	/* Cliente conectado. Esperando comando play */
	PLAY,		/* Jugando el nivel */
	PAUSE,	/* Nivel pausado */
	END		/* Juego terminado */
}	status_t;

typedef struct t_game{
	status_t status;
	uint32_t score;
	ship_t *player;
	lista_t *enemies;
	lista_t *shoot_enemies;
	lista_t *shoot_player;
	level_t *level;
} game_t;

void game_init(game_t *game);
void game_run(game_t *game);
void game_set_level(game_t *game, uint16_t level);
void game_start(game_t *game);
void game_pause(game_t *game);
void game_over(game_t *game);

#endif
