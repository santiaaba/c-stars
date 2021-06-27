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
   newGame->enemies = lista_init();
   newGame->shot_enemies = lista_init();
   newGame->shot_player = lista_init();
   return newGame
}

void game_play(game_t *game){
	/* verificamos cambio en teclado */

	/* Gestionamos enemigos */
	lista_begin(game->enemies)
	while(!lista_eol(game->enemies)){
		ship_move(lista_get(game->enemies))
		/* Calculamos colision con enemigos */
		ship_colision(game->player,lista_get(game->enemies))
		/* Disparamos si corresponde */
		ship_shoot(game->player,game->shot_enemies)
		lista_next(game->enemies)
	}
	/* Movemos disparos jugador */
	lista_begin(game->shoot_player)
	while(!lista_eol(game->shoot_player)){
		shoot_move(lista_get(game->shoot_player))
		/* Calculamos colision con enemigos */
		lista_begin(game->enemies)
		while(!lista_eol(game->enemies)){
			ship_colision(	lista_get(game->enemies),
								lista_get(game->shoot_player))
			lista_next(game->enemies)
		}
		lista_next(game->shoot_player)
	}
	/* Movemos disparos enemigos */
	lista_begin(game->shoot_enemies)
	while(!lista_eol(game->shoot_enemies)){
		shoot_move(lista_get(game->shoot_enemies))
		/* Calculamos colision con jugador */
		ship_colision(player,lista_get(game->shoot_enemies))
		lista_next(game->shoot_enemies)
	}
	/* Dibujamos pantalla */
	/* Enviamos por UDP los datos para el render */
	/* Enviamos por UDP los datos para el sonido */

	/* Enviamos datos del juego por TCP */
}

void game_run(game_t *game){
	while (true){
		switch(game->status){
			case INIT:
			case READY:
			case PLAY:
				game_play(game);
				break;
			case PAUSE:
			case END:
		}
	}
}
