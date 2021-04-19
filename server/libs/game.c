#include "libs/point.h"
#include "libs/ship.h"
#include "libs/shoot.h"
#include "libs/game.h"

void game_init(){
   game_t *newGame;

   newGame = (game_t *)malloc(sizeof(game_t));
	newGame -> status = INIT;
	newGame -> score = 0;
   newGame->player = ship_init();
   newGame->enemies = list_init();
   newGame->shot_enemies = list_init();
   newGame->shot_player = list_init();
   return newGame
}

void game_start(game_t *game){
	while(game->status == PLAY){
		/* Capturamos presión tecla */

		/* Movemos enemigos */
		list_begin(game->enemies)
		while(!list_eol(game->enemies)){
			ship_move(list_get(game->enemies))
			/* Calculamos colision con enemigos */
			ship_colision(game->player,list_get(game->enemies))
			/* Disparamos si corresponde */
			ship_shoot(game->player,game->shot_enemies)
			list_next(game->enemies)
		}
		/* Movemos disparos jugador */
		list_begin(game->shoot_player)
		while(!list_eol(game->shoot_player)){
			shoot_move(list_get(game->shoot_player))
			/* Calculamos colision con enemigos */
			list_begin(game->enemies)
			while(!list_eol(game->enemies)){
				ship_colision(	list_get(game->enemies),
									list_get(game->shoot_player))
				list_next(game->enemies)
			}
			list_next(game->shoot_player)
		}
		/* Movemos disparos enemigos */
		list_begin(game->shoot_enemies)
		while(!list_eol(game->shoot_enemies)){
			shoot_move(list_get(game->shoot_enemies))
			/* Calculamos colision con jugador */
			ship_colision(player,list_get(game->shoot_enemies))
			list_next(game->shoot_enemies)
		}
		/* Dibujamos pantalla */
	}
}
