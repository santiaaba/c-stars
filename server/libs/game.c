#include "game.h"

void game_init(game_t *game){
	game->status = WAIT;
	game->score = 0;
   game->player = (ship_t *)malloc(sizeof(ship_t));
	game->enemies = (lista_t *)malloc(sizeof(lista_t));
   game->shoot_enemies = (lista_t *)malloc(sizeof(lista_t));
   game->shoot_enemies = (lista_t *)malloc(sizeof(lista_t));

	ship_init(game->player,HEROE);
	lista_init(game->enemies,sizeof(ship_t));
	lista_init(game->shoot_enemies,sizeof(shoot_t));
	lista_init(game->shoot_player,sizeof(shoot_t));
}

void game_set_level(game_t *game, uint16_t idLevel){
	level_destroy(game->level);
	level_set(game->level,idLevel);
}

static void game_play(game_t *game){
	/* verificamos cambio en teclado */

	/* Gestionamos enemigos */
	lista_first(game->enemies);
	while(!lista_eol(game->enemies)){
		/* movemos la nave del enemigo */
		ship_move(lista_get(game->enemies));

		/* Calculamos colision con jugador */
		ship_colision_ship(lista_get(game->enemies),game->player);

		/* Calculamos colision con disparos jugador */
		lista_first(game->shoot_player);
		while(!lista_eol(game->shoot_player)){
			ship_colision_shoot(	lista_get(game->enemies),
										lista_get(game->shoot_player));
			lista_next(game->shoot_player);
		}

		/* Disparamos */
		//ship_shoot(game->player,game->shoot_enemies);

		lista_next(game->enemies);
	}
	/* Gestionamos disparos jugador */
	lista_first(game->shoot_player);
	while(!lista_eol(game->shoot_player)){
		shoot_move(lista_get(game->shoot_player));
		/* Calculamos colision con enemigos */
		/* Esto quizas pueda eliminarse ya que se
	      realiza cuando se gestionan los enemigos */

		lista_first(game->shoot_player);
		while(!lista_eol(game->shoot_player)){
			ship_colision_shoot(	lista_get(game->enemies),
										lista_get(game->shoot_player));
			lista_next(game->shoot_player);
		}

		lista_next(game->shoot_player);
	}

	/* Movemos disparos enemigos */
	lista_first(game->shoot_enemies);
	while(!lista_eol(game->shoot_enemies)){
		shoot_move(lista_get(game->shoot_enemies));
		/* Calculamos colision con jugador */
		ship_colision_shoot(game->player,lista_get(game->shoot_enemies));
		lista_next(game->shoot_enemies);
	}

	/* Movemos al jugador */
	ship_move(game->player);

	/* Lanzamos nuevos enemigos si corresponde */
	level_run(game->level,game->enemies);

	/* Enviamos por UDP los datos para el render */
	/* Enviamos por UDP los datos para el sonido */
}

void game_run(game_t *game){
	while (1){
		switch(game->status){
			case WAIT:
				break;
			case READY:
				break;
			case PLAY:
				game_play(game);
				break;
			case PAUSE:
				break;
			case END:
				break;
		}
	}
}
