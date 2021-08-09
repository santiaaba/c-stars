#include "game.h"

void game_init(game_t *game, sem_t *sem){
	game->status = READY;
	game->score = 0;
	game->sem = sem;
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
				vector_set(vector,90,ship_speed(game->player));
				break;
		   case TOPRIGHT:
				vector_set(vector,45,ship_speed(game->player));
				break;
		   case RIGHT:
				vector_set(vector,0,ship_speed(game->player));
				break;
		   case RIGHTBOTTOM:
				vector_set(vector,315,ship_speed(game->player));
				break;
		   case BOTTOM:
				vector_set(vector,270,ship_speed(game->player));
				break;
		   case BOTTOMLEFT:
				vector_set(vector,225,ship_speed(game->player));
				break;
		   case LEFT:
				vector_set(vector,180,ship_speed(game->player));
				break;
		   case LEFTTOP:
				vector_set(vector,135,ship_speed(game->player));
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

		/* render info para cliente */
		ship_render(lista_get(game->enemies),game->buffer[game->buffer_size]);
		buffer_size += DATA_RENDER_SIZE;
		if(buffer_size + DATA_RENDER_SIZE >= BUFFER_SIZE)
			game_client_send(game);

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

		/* render info para cliente */
		shoot_render(lista_get(game->shoot_player),game->buffer[game->buffer_size]);
		game->buffer_size += DATA_RENDER_SIZE;
		if(game->buffer_size + DATA_RENDER_SIZE >= BUFFER_SIZE)
			game_client_send(game);

		lista_next(game->shoot_player);
	}

	/* Movemos disparos enemigos */
	lista_first(game->shoot_enemies);
	while(!lista_eol(game->shoot_enemies)){
		shoot_move(lista_get(game->shoot_enemies));
		/* Calculamos colision con jugador */
		ship_colision_shoot(game->player,lista_get(game->shoot_enemies));
	
		shoot_render(lista_get(game->shoot_player),game->buffer[game->buffer_size]);
		game->buffer_size += DATA_RENDER_SIZE;
		if(game->buffer_size + DATA_RENDER_SIZE >= BUFFER_SIZE)
			game_client_send(game);
	
		lista_next(game->shoot_enemies);
	}

	/* Si quedan datos en el buffer, se envían */
	if(game->buffer_size > 0)
			game_client_send(game);

	/* Movemos al jugador */
	ship_move(game->player);

	/* Lanzamos nuevos enemigos si corresponde */
	level_run(game->level,game->enemies);

	game_render(game)
}

void game_streming(game_t *game){
	/* Envia el buffer por udp */
	while(1){
		sem_wait(game->sem_buffer);
		if(game->pre_buffer_size > 0){
			memcpy(game->udp_buffer,game->pre_buffer,game->pre_buffer_size);
			game->udp_buffer_size = game->pre_buffer_size;
			game->pre_buffer_size = 0;
		}
		sem_post(game->sem_buffer);
		if(game->udp_buffer_size > 0){
			send(game->clientfd, game->udp_buffer, game->udp_buffer_size);
			game->udp_buffer_size = 0;
		}
	}
}

void game_render(game_t *game){
	/* Arma el buffer de donde luego consume el hilo cliente UDP
		para enviar al cliente. El buffer siempre es pisado. */
	/* El acceso al buffer y a su tamano es seccion critica */
	
	sem_wait(game->sem_buffer);
	/* Render jugador */
	ship_render(game->ship)
	/* Render disparos jugador */
	lista_first(game->)
		/* Obtener la posicion */
		/* Obtener el tipo */
		/* Obtener el sprite */
		/* Obtener el frame */
	/* Render enemigos */
	lista->first(game->enemies);
	while(!lista_eol(game->enemies)){
		/* Obtener la posicion */
		/* Obtener el tipo */
		/* Obtener el sprite */
		/* Obtener el frame */
		ship_get_position(lista_get(game->enemies));
		lista_next(game->enemies);
	}
	/* Render disparos enemigos */
	while(!lista_eol(game->shoot_enemies)){
		/* Obtener la posicion */
		/* Obtener el tipo */
		/* Obtener el sprite */
		/* Obtener el frame */
	}
	sem_post(game->sem_buffer);
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
