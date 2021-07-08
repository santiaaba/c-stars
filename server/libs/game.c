#include "game.h"

void game_init(game_t *game){
	game->status = READY;
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

void game_key(game_t *game){
	int i = 0;
	vector_t *vector;
	while(i < game->key_size){
		vector = ship_get_vector(game->player);
		switch(game->key[i]){
			case CERO:
				vector_set(vector,0,0);
				break;
			case TOP:
				vector_set(vector,averiguar,1);
				break;
		   case TOPRIGHT:
				vector_set(vector,averiguar,1);
				break;
		   case RIGHT:
				vector_set(vector,averiguar,1);
				break;
		   case RIGHTBOTTOM:
				vector_set(vector,averiguar,1);
				break;
		   case BOTTOM:
				vector_set(vector,averiguar,1);
				break;
		   case BOTTOMLEFT:
				vector_set(vector,averiguar,1);
				break;
		   case LEFT:
				vector_set(vector,averiguar,1);
				break;
		   case LEFTTOP:
				vector_set(vector,averiguar,1);
				break;

		   case FIRE_PRESS:
				break;
		   case FIRE_RELEASE:
				break;
		   case PAUSE_PRESS:
				game->status = PAUSE;
				break;
		   case START_PRESS:
				game->status = PLAYING;
				break;
		}
	}
	game->key_size = 0;
}

static void game_play(game_t *game){
	/* verificamos cambio en teclado */
	/* Leemos de un listado de acciones */
	game_key(game)

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
	while (game->status != LEAVE){
		switch(game->status){
			case PLAYING:
				game_play(game);
				break;
			case PAUSE:
				break;
			case END:
				break;
		}
		nanosleep(60000);
	}
}
