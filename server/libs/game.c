#include "game.h"

void game_init(game_t *g, sem_t *sem_event){
	int i;

	g->event_size = 0;
	for(i=0;i<EVENT_LIMIT_SIZE;i++)
		g->events[i] = NULL;

	printf("Llego\n");
	g->status = G_WAIT_CONNECT;
	g->score = 0;
	g->sem_event = sem_event;
   g->player = (ship_t *)malloc(sizeof(ship_t));
	g->enemies = (lista_t *)malloc(sizeof(lista_t));
   g->shoot_enemies = (lista_t *)malloc(sizeof(lista_t));
   g->shoot_enemies = (lista_t *)malloc(sizeof(lista_t));
	printf("Llego\n");

	ship_init(g->player,PLAYER, g->clock);
	lista_init(g->enemies,sizeof(ship_t));
	lista_init(g->shoot_enemies,sizeof(shoot_t));
	lista_init(g->shoot_player,sizeof(shoot_t));
	printf("Llego\n");

	clockgame_init(g->clock);
	printf("Llego\n");
	
	/* Inicializamos los semaforos */
	sem_post(g->sem_event);
	printf("Llego\n");
}

void game_set_level(game_t *g, int idLevel){
	level_destroy(&(g->level));
	level_init(&(g->level),idLevel,g->clock);
}

void game_event_add(game_t *g, game_event_t *e){
	/* Entramos en seccion critica */
	sem_wait(g->sem_event);
	if(g->event_size < EVENT_LIMIT_SIZE){
		g->event_size++;
		g->events[g->event_size] = e;
	}
	/* Salimos de seccion critica */
	sem_post(g->sem_event);
}

void static game_handle_events(game_t *g){
	int i = 0;
	vector_t *vector;
	/* Entramos en seccion critica */
	sem_wait(g->sem_event);
	for(i=0;i < g->event_size;i++){
		vector = ship_get_vector(g->player);
		switch(g->events[i]->key_type){
			case K_DOWN:
				switch(g->events[i]->key){
					case K_TOP:
						g->direction.top = true;
						break;
					case K_BOTTOM:
						g->direction.bottom = true;
						break;
					case K_LEFT:
						g->direction.left = true;
						break;
					case K_RIGHT:
						g->direction.right = true;
						break;
					case K_SPACEBAR:
						break;
				}
				break;
			case K_UP:
				switch(g->events[i]->key){
					case K_TOP:
						g->direction.top = false;
						break;
					case K_BOTTOM:
						g->direction.bottom = false;
						break;
					case K_LEFT:
						g->direction.left = false;
						break;
					case K_RIGHT:
						g->direction.right = false;
						break;
					case K_SPACEBAR:
						break;
				}
		}
		free(g->events[i]);
	}
	
	g->event_size = 0;

	/* Salimos de seccion critica */
	sem_post(g->sem_event);
}

void static game_send_udp(game_t *g){
	/* Arma el buffer UDP y envia los datos */
}

static void game_play(game_t *g){

	render_t *r;
	int i = 0;

	/* Leemos de un listado de acciones */
	game_handle_events(g);

	/* Gestionamos enemigos */
	lista_first(g->enemies);
	while(!lista_eol(g->enemies)){
		/* movemos la nave del enemigo */
		ship_move(lista_get(g->enemies));

		/* Calculamos colision con jugador */
		ship_colision_ship(lista_get(g->enemies),g->player);

		/* Calculamos colision con disparos jugador */
		lista_first(g->shoot_player);
		while(!lista_eol(g->shoot_player)){
			ship_colision_shoot(	lista_get(g->enemies),
										lista_get(g->shoot_player));
			lista_next(g->shoot_player);
		}

		/* render info para cliente */
		ship_render(lista_get(g->enemies),&(g->buffer[i]));
		i++;
		/* Disparamos */
		//ship_shoot(g->player,g->shoot_enemies);

		lista_next(g->enemies);
	}
	/* Gestionamos disparos jugador */
	lista_first(g->shoot_player);
	while(!lista_eol(g->shoot_player)){
		shoot_move(lista_get(g->shoot_player));
		/* Calculamos colision con enemigos */
		/* Esto quizas pueda eliminarse ya que se
	      realiza cuando se gestionan los enemigos */

		lista_first(g->shoot_player);
		while(!lista_eol(g->shoot_player)){
			ship_colision_shoot(	lista_get(g->enemies),
										lista_get(g->shoot_player));
			lista_next(g->shoot_player);
		}

		/* render info para cliente */
		shoot_render(lista_get(g->shoot_player),&(g->buffer[i]));
		i++;
		lista_next(g->shoot_player);
	}

	/* Movemos disparos enemigos */
	lista_first(g->shoot_enemies);
	while(!lista_eol(g->shoot_enemies)){
		shoot_move(lista_get(g->shoot_enemies));
		/* Calculamos colision con jugador */
		ship_colision_shoot(g->player,lista_get(g->shoot_enemies));
		shoot_render(lista_get(g->shoot_player),&(g->buffer[i]));
		i++;
		lista_next(g->shoot_enemies);
	}

	/* Movemos al jugador */
	ship_move(g->player);
	//r = (render_t *)malloc(sizeof(render_t));
	ship_render(g->player,&(g->buffer[i]));

	/* Lanzamos nuevos enemigos si corresponde */
	level_run(g->level,g->enemies);

	game_send_udp(g);
}

int game_get_state(game_t *g){
	return g->status;
}

void game_set_state(game_t *g, int state){
	g->status = state;
}

void game_run(game_t *g){
	/* funcion principal a entregar al hilo del juego */
	while (g->status != G_LEAVE){
		switch(g->status){
			case G_PLAYING:
				/* Juego en ejecucion */
				game_play(g);
				break;
			case G_READY:
				/* Juego listo */
				break;
			case G_PAUSE:
				/* Juego pausado */
				break;
			case G_STOP:
				/* Juego terminado */
				break;
		}
		usleep(NANOTIME);
		//nanosleep(NANOTIME);
	}
}
