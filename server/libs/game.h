#ifndef GAME_H
#define GAME_H

#include "point.h"
#include "ship.h"
#include "shoot.h"
#include <stdint.h>
#include <time.h>

typedef enum {
   TOP_PRESS,
   TOP_RELEASE,
   BOTTOM_PRESS,
   BOTTOM_RELEASE,
   LEFT_PRESS,
   LEFT_RELEASE,
   RIGHT_PRESS,
   RIGHT_RELEASE,
   FIRE_PRESS,
   FIRE_RELEASE,
   PAUSE_PRESS,
   START_PRESS
} key_t;

typedef enum {
	READY,				/* Cliente conectado. Esperando comando play */
	PLAYING,				/* Jugando el nivel */
	PAUSE,				/* Nivel pausado */
	END,					/* Juego terminado */
	LEAVE					/* Finalizar juego. Salir del programa */
}	status_t;

typedef struct t_game{
	key_t key[10];
	key_size;		/* Tamano logico del vector key */
	status_t status;
	uint32_t score;
	ship_t *player;
	lista_t *enemies;
	lista_t *shoot_enemies;
	lista_t *shoot_player;
	level_t *level;
} game_t;

void game_init(game_t *game);
void game_key(game_t *game);
void game_run(game_t *game);
void game_set_level(game_t *game, uint16_t level);
void game_start(game_t *game);
void game_pause(game_t *game);
void game_over(game_t *game);

#endif
