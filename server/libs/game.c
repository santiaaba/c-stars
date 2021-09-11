#include "game.h"

void game_init(game_t *g, sem_t *sem_event){
	int i;

	g->event_size = 0;
	for(i=0;i<EVENT_LIMIT_SIZE;i++)
		g->events[i] = NULL;

	g->status = G_WAIT_CONNECT;
	g->score = 0;
	g->sem_event = sem_event;
	sem_init(&(g->sem_status),0,1);

   g->player = (ship_t *)malloc(sizeof(ship_t));
	g->enemies = (lista_t *)malloc(sizeof(lista_t));
   g->shoot_enemies = (lista_t *)malloc(sizeof(lista_t));
   g->shoot_player = (lista_t *)malloc(sizeof(lista_t));
	g->clock = (clockgame_t *)malloc(sizeof(clockgame_t));

	ship_init(g->player,PLAYER, g->clock);
	lista_init(g->enemies,sizeof(ship_t));
	lista_init(g->shoot_enemies,sizeof(shoot_t));
	lista_init(g->shoot_player,sizeof(shoot_t));

	clockgame_init(g->clock);
	
	/* Inicializamos los semaforos */
	//sem_post(g->sem_event);
}


void game_level_start(game_t *g, int idLevel){
	/* Daja preparado el juego para el nivel en cuestion */

	level_destroy(&(g->level));
	level_init(&(g->level),idLevel,g->clock);
	lista_clean(g->shoot_enemies,&shoot_destroy);
	lista_clean(g->shoot_player,&shoot_destroy);
	lista_clean(g->enemies,&ship_destroy);
	ship_set_position(g->player,100,300);
	g->status = G_PLAYING;
}

void game_start(game_t *g){
	/* Resetea el juego y lo inicia en el nivel 1 */
	g->score = 0;
	game_level_start(g,1);
	game_set_status(g,G_PLAYING);
}

void game_stop(game_t *g){
	/* Finaliza el juego. No implica mucho.
		Solo salir del bucle y no ingresar a el
		nuevamente salvo que se realice un game_start */
		game_set_status(g,G_STOP);
}

void game_pause(game_t *g){
	/* Pausa el juego. No implica mucho.
	   Solo salir del bucle e ingresar a el
		nuevamente con un game_resume */
		game_set_status(g,G_PAUSE);
}

void game_resume(game_t *g){
	/* Pausa el juego. No implica mucho.
	   Solo salir ingresar nuevamente al bucle */
		game_set_status(g,G_PLAYING);
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
	char buffer[MAX_UDP_BUFFER];
	int i,j = 0;

	while(i < g->buffer_size){
		j = 0;
		while(j < MAX_UDP_BUFFER){
			g->buffer[i];
			i++;
			j++;
		}
		/* Luego de armados los datos, los enviamos */
		sendto(g->sockfd, &buffer, strlen(buffer),
			0, (const struct sockaddr *) &(g->servaddr), 
			sizeof(g->servaddr));
	}
}

int game_init_udp(game_t *g, char *ip, int port){
	if ( (g->sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		g->status = G_WAIT_CONNECT;
		return 0;
	}
  
	memset(&(g->servaddr), 0, sizeof(g->servaddr));
	g->servaddr.sin_family = AF_INET;
	g->servaddr.sin_port = htons(port);
	g->servaddr.sin_addr.s_addr = inet_addr(ip);

	g->status = G_READY;
	return 1;
}

void static game_ingress_level(game_t *g){
	/* Bucle de la animacion que presenta
		el nivel. Muestra la nave del jugador
		ingresando por la zquierda de la pantalla */

}

void static game_egress_level(game_t *g){
	/* Bucle de la animacion que finaliza
	   el nivel. Muestra a la nave del jugador
		saliendo por la derecha de la pantalla */
}

void static game_playing_level(game_t *g){
	/* Bucle que posee la logica del juego.
		GENERA un FRAME del juego  */

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

void game_set_state(game_t *g, int status){
	sem_wait(g->sem_status);
		g->status = status;
	sem_post(g->sem_status);
}

void game_run(game_t *g){
	/* funcion principal a entregar al hilo del juego */
	while (g->status != G_LEAVE){
		switch(g->status){
			case G_PLAYING:
				game_ingress_level(g);
				game_playing_level(g);
				game_egress_level(g);
			case G_
		usleep(NANOTIME);
	}
}
