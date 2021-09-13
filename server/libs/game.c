#include "game.h"

void game_init(game_t *g, sem_t *sem_event){
	int i;

	printf("Iniciamos estructura del juego\n");
	g->event_size = 0;
	for(i=0;i<EVENT_LIMIT_SIZE;i++)
		g->events[i] = NULL;

	printf("ACA 1 game\n");
	g->state = G_WAIT_CONNECT;
	g->score = 0;
	printf("ACA 1 game\n");
	g->sem_event = sem_event;	// Debe venir ya inicializado
	printf("ACA 1 game\n");
	sem_init(&(g->sem_state),0,1);
	printf("ACA 2 game\n");

   g->player = (ship_t *)malloc(sizeof(ship_t));
	g->enemies = (lista_t *)malloc(sizeof(lista_t));
   g->shoot_enemies = (lista_t *)malloc(sizeof(lista_t));
   g->shoot_player = (lista_t *)malloc(sizeof(lista_t));
	g->clock = (clockgame_t *)malloc(sizeof(clockgame_t));
	g->request_status = 0;
	printf("ACA 3 game\n");

	ship_init(g->player,PLAYER, g->clock);
	printf("ACA 4 game\n");
	lista_init(g->enemies,sizeof(ship_t));
	printf("ACA 5 game\n");
	lista_init(g->shoot_enemies,sizeof(shoot_t));
	printf("ACA 6 game\n");
	lista_init(g->shoot_player,sizeof(shoot_t));
	printf("ACA 7 game\n");

	clockgame_init(g->clock);
	printf("ACA 8 game\n");
	
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
	g->frame = 0;

	game_set_state(g,G_PLAYING);
}

void game_start(game_t *g){
	/* Resetea el juego y lo inicia en el nivel 1 */
	g->score = 0;
	game_level_start(g,1);
}

void game_stop(game_t *g){
	/* Finaliza el juego. No implica mucho.
		Solo salir del bucle y no ingresar a el
		nuevamente salvo que se realice un game_start */
		game_set_state(g,G_STOP);
}

void game_pause(game_t *g){
	/* Pausa el juego. No implica mucho.
	   Solo salir del bucle e ingresar a el
		nuevamente con un game_resume */
		game_set_state(g,G_PAUSE);
}

void game_resume(game_t *g){
	/* Pausa el juego. No implica mucho.
	   Solo salir ingresar nuevamente al bucle */
		game_set_state(g,G_PLAYING);
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
	char *buffer;
	int i, j, body_size = 0;
	int buffer_size;
	data_t data;

	data.header.frame = g->frame;
	data.header.type = D_VIDEO;
	data.header.aux = 0 || g->request_status;
	data.header.size = sizeof(data_render_t) * g->buffer_cant;
	/* Enviamos todos los elementos de video */
	while(i < g->buffer_cant){
		/* Armamos el body hasta su capasidad maxima */
		j = 0;
		while((body_size + DATA_ENTITY_SIZE) < MAX_UDP_BUFFER){
			data_entity_copy(&(data.body[j]),g->buffer[i]);
			i++;
			j++;
		}
		/* Luego de armados los datos, los convertimos a buffer char */
		data_to_buffer(&data,&buffer,&buffer_size);
		/* Luego los enviamos */
		sendto(g->sockfd, &buffer, buffer_size,
			0, (const struct sockaddr *) &(g->servaddr), 
			sizeof(g->servaddr));
	}
}

int game_init_udp(game_t *g, char *ip, int port){
	if ( (g->sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		g->state = G_WAIT_CONNECT;
		return 0;
	}
  
	memset(&(g->servaddr), 0, sizeof(g->servaddr));
	g->servaddr.sin_family = AF_INET;
	g->servaddr.sin_port = htons(port);
	g->servaddr.sin_addr.s_addr = inet_addr(ip);

	g->state = G_READY;
	return 1;
}

void static game_playing_level(game_t *g){
	/* Bucle que posee la logica del juego.
		GENERA un FRAME del juego  */

	int i = 0;

	/* Leemos de un listado de acciones */

	while(g->state == G_PLAYING){

		if(level_get_state(g->level) == L_PLAYING)
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
		i++;
		g->buffer_cant = i;
	
		/* Lanzamos nuevos enemigos si corresponde */
		level_run(g->level,g->enemies);

		switch(level_get_state(g->level)){
			case L_INGRESS:
				/* Si level esta en L_INGRESS, lo pasamos a L_PLAYING
					cuando la nave del jugador esté a 20 pixeles del lado
					izquierdo */
				if(point_get_x(ship_get_position(g->player)) >= 100)
					level_set_state(g->level,L_PLAYING);
				break;
			case L_PLAYING:
				if((g->level) && lista_size(g->enemies) == 0 &&
					lista_size(g->shoot_enemies) == 0){
						level_set_state(g->level,L_EGRESS);
				}
				break;
			case L_EGRESS:
				/* Cuando la nave sale de la pantalla hemos terminado
					el nivel */
				if(point_get_x(ship_get_position(g->player)) >= 900){
					level_set_state(g->level,L_END);
					/* colocamos en 1 request_status para que viaje
						el próximo udp con el bit encendido que avisa
						al cliente que requiere realizar un game_status */
					g->request_status = 1;
				}
		}

		game_send_udp(g);
	}
}

void game_status(game_t *g){
	/* Retorna información del estado del juego.
		Se coloca el parametro que lo fuerza en 0 */
	g->request_status = 0;
	
}

int game_get_state(game_t *g){
	return g->state;
}

void game_set_state(game_t *g, int state){
	sem_wait(&(g->sem_state));
		g->state = state;
	sem_post(&(g->sem_state));
}

void game_info(game_t *g, game_info_t *info){
	info->score = g->score;
	info->state = g->state;
	info->level = level_get_id(g->level);
	info->level_state = level_get_state(g->level);
}

void *game_run(void *g){
	/* funcion principal a entregar al hilo del juego */
	while (((game_t *)g)->state != G_LEAVE){
		switch(((game_t *)g)->state){
			case G_PLAYING:
				game_playing_level((game_t *)g);
		}
	}
}
