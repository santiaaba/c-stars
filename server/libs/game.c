#include "game.h"

void game_init(game_t *g, sem_t *sem_event){
	int i;

	printf("Iniciamos estructura del juego\n");
	g->event_size = 0;
	for(i=0;i<EVENT_LIMIT_SIZE;i++)
		g->events[i] = NULL;

	g->state = G_WAIT_CONNECT;
	g->score = 0;
	g->sem_event = sem_event;	// Debe venir ya inicializado
	sem_init(&(g->sem_state),0,1);

   g->player = (ship_t *)malloc(sizeof(ship_t));
	g->clock = (clockgame_t *)malloc(sizeof(clockgame_t));
	g->request_status = 0;
	g->statusForced = false;

	g->level=(level_t*)malloc(sizeof(level_t));
	level_init(g->level,g->clock);

	g->enemies = (lista_t*)malloc(sizeof(lista_t));
	g->shoot_enemies = (lista_t*)malloc(sizeof(lista_t));
	g->shoot_player = (lista_t*)malloc(sizeof(lista_t));

	ship_init(g->player,PLAYER, g->clock);
	ship_set_limits(g->player,SCREEN_HEIGHT,SCREEN_WIDTH);

	lista_init(g->enemies,sizeof(ship_t));
	lista_init(g->shoot_enemies,sizeof(shoot_t));
	lista_init(g->shoot_player,sizeof(shoot_t));

	clockgame_init(g->clock);

	g->data.header.size = 0;
	g->buffer = (char*)malloc(sizeof(char)*8);
}

void game_level_start(game_t *g, int idLevel){
	/* Daja preparado el juego para el nivel en cuestion */

	printf("game_level_start(): inicio level %i\n", idLevel);
	level_load(g->level,idLevel);
	lista_clean(g->shoot_enemies,&shoot_destroy);
	lista_clean(g->shoot_player,&shoot_destroy);
	lista_clean(g->enemies,(void*)(void**)&ship_destroy);
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

void game_event_add(game_t *g, uint16_t key, uint16_t key_type){
	game_event_t *e;

	e = (game_event_t*)malloc(sizeof(game_event_t));
	e->key = key;
	e->key_type = key_type;
	/* Entramos en seccion critica */
	sem_wait(g->sem_event);
	if(g->event_size < EVENT_LIMIT_SIZE){
		g->events[g->event_size] = e;
		g->event_size++;
	}
	/* Salimos de seccion critica */
	sem_post(g->sem_event);
}

void static game_handle_events(game_t *g){
	int i = 0;
	float direction = 0;
	float module = 0;
	vector_t *vector;
	/* Entramos en seccion critica */
	sem_wait(g->sem_event);
	for(i=0;i < g->event_size;i++){
		printf("game_handle_events(): KEY: %i - EVENT: %i\n",g->events[i]->key,g->events[i]->key_type);
		vector = ship_get_vector(g->player);
		switch(g->events[i]->key_type){
			case K_DOWN:
				printf("game_handle_events(): KEY_DOWN\n");
				switch(g->events[i]->key){
					case K_TOP:
						printf("game_handle_events(): KEY_DOWN K_TOP\n");
						g->direction.top = true;
						break;
					case K_BOTTOM:
						printf("game_handle_events(): KEY_DOWN K_BOTTOM\n");
						g->direction.bottom = true;
						break;
					case K_LEFT:
						printf("game_handle_events(): KEY_DOWN K_LEFT\n");
						g->direction.left = true;
						break;
					case K_RIGHT:
						printf("game_handle_events(): KEY_DOWN K_RIGHT\n");
						g->direction.right = true;
						break;
					case K_SPACEBAR:
						printf("game_handle_events(): KEY_DOWN SPACE\n");
						break;
				}
				break;
			case K_UP:
				printf("game_handle_events(): KEY_UP\n");
				switch(g->events[i]->key){
					case K_TOP:
						printf("game_handle_events(): KEY_UP K_TOP\n");
						g->direction.top = false;
						break;
					case K_BOTTOM:
						printf("game_handle_events(): KEY_UP K_BOTTOM\n");
						g->direction.bottom = false;
						break;
					case K_LEFT:
						printf("game_handle_events(): KEY_UP K_LEFT\n");
						g->direction.left = false;
						break;
					case K_RIGHT:
						printf("game_handle_events(): KEY_UP K_RIGHT\n");
						g->direction.right = false;
						break;
					case K_SPACEBAR:
						printf("game_handle_events(): KEY_UP SPACE\n");
						break;
				}
		}
		if(g->direction.left || g->direction.right ||
		   g->direction.top || g->direction.bottom)
				module = PLAYER_MODULE;
		else
				module = 0;

		if(g->direction.left)
			direction = G_180;	//180°
		if(g->direction.top)
			direction = G_90;	//90°
		if(g->direction.right)
			direction = G_0;		//0°
		if(g->direction.bottom)
			direction = G_270;	//270°
		if(g->direction.left && g->direction.top)
			direction = G_135;	//135°
		if(g->direction.right && g->direction.top)
			direction = G_45;		//45°
		if(g->direction.left && g->direction.bottom)
			direction = G_225;	//225°
		if(g->direction.right && g->direction.bottom)
			direction = G_315;	//315°
		printf("game_handle_events(): (M,D) => (%f,%f)\n",module,direction);
		vector_set(vector,direction,module);
		free(g->events[i]);
	}
	
	g->event_size = 0;

	/* Salimos de seccion critica */
	sem_post(g->sem_event);
}

void static game_send_data(game_t *g, data_render_t *data, bool at_once){
	/* Va armando el listado de datos a ser enviados por udp */
	/* Si at_once es true se envian inmediatamente. Sino se almacenan
		para ser enviados luego. Si no queda espacio de almacenamiento
		entonces los envia inmediatamente. */
	/* Data puede ser NULL si se desea enviar los datos ya almacenados
		y no se está sumando un dato nuevo (data == NULL)*/

	int n;

//	printf("game_send_data()-header.size=%u\n",g->data.header.size);
	if(data != NULL){
		data_entity_copy(&(g->data.body[g->data.header.size]),data);
		g->data.header.size ++;
	}

	if(at_once || g->data.header.size == MAX_DATA_BODY){
		g->data.header.frame = g->frame;
		g->data.header.type = D_VIDEO;
		if(g->request_status)
			g->data.header.aux = AUX_FORCESTATUS;
		else
			g->data.header.aux = 0;

		data_to_buffer(&(g->data),&(g->buffer),&(g->buffer_size));
//		printf("game_send_data(): Bytes a enviar: %i\n",g->buffer_size);
		n = sendto(g->sockfd, g->buffer, g->buffer_size, 0,
			(const struct sockaddr *) &(g->servaddr),sizeof(g->servaddr));
//		printf("game_send_data(): Bytes enviados: %i\n",n);
		g->data.header.size = 0;
	}
}

int game_init_udp(game_t *g, char *ip, int port){
	if ( (g->sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		g->state = G_WAIT_CONNECT;
		return 0;
	}
  
	printf("game_init_udp(): puerto = %i\n",port);
	memset(&(g->servaddr), 0, sizeof(g->servaddr));
	g->servaddr.sin_family = AF_INET;
	g->servaddr.sin_port = htons(port);
	inet_aton(ip, &(g->servaddr.sin_addr));
	//g->servaddr.sin_addr.s_addr = ip;

	g->state = G_READY;
	return 1;
}

void static game_playing_level(game_t *g){
	/* Bucle que posee la logica del juego.
		GENERA un FRAME del juego  */

	struct timespec req;
	struct timespec rem;
	int cantfotograms;
	ship_t *ship;
	data_render_t data;

	req.tv_sec = (1000/FXS) / 1000;
	req.tv_nsec = ((1000/FXS) % 1000) * 1000000;

	/* Leemos de un listado de acciones */

	printf("game_playing_level(): Arrancamos el bucle\n");

	g->data.header.size = 0;
	while(g->state == G_PLAYING){
	//	continue;

		if(level_get_state(g->level) == L_PLAYING)
			game_handle_events(g);
	//	printf("game_playing_level(): RELOJ: %u\n",clockgame_time(g->clock));
		/* Gestionamos enemigos */
	//	printf("game_playing_level(): Gestionamos enemigos (%i)\n",
//			lista_size(g->enemies));
		lista_first(g->enemies);
		while(!lista_eol(g->enemies)){
	//		printf("game_playing_level(): Nave enemiga\n");
			ship = lista_get(g->enemies);
			switch(ship_get_state(ship)){
				case SHIP_LIVE:
	//				printf(" Movemos\n");
					ship_move(ship);
					/* Calculamos colision con jugador */
	//				printf("	Colisiones con jugador\n");
					if(ship_colision_ship(ship,g->player)){
	//					printf("Chocan nava enemiga y jugador!!!\n");
						/* Decrementamos energia jugador */
						ship_set_power(g->player,(ship_get_power(g->player) -
							ship_get_power(ship)));
						if(ship_get_power(g->player) <= 0){
							ship_set_state(g->player,SHIP_DESTROY);
						}
						/* Comenzamos eliminacion nave enemiga */
						ship_set_state(ship,SHIP_DESTROY);
						ship_set_animation(ship,1,15,false);
					} else {
						/* Calculamos colision con disparos jugador */
						lista_first(g->shoot_player);
	//					printf("	Colisiones con disparos\n");
						while(!lista_eol(g->shoot_player)){
							ship_colision_shoot(ship,lista_get(g->shoot_player));
							lista_next(g->shoot_player);
						}
					}
					//ship_shoot(g->player,g->shoot_enemies);
					ship_render(ship,&data);
					game_send_data(g,&data,false);
					lista_next(g->enemies);
					break;
				case SHIP_DESTROY:
	//				printf(" Destruimos\n");
					if(animation_end(&(ship->animation)))
						ship_set_state(ship,SHIP_END);
					else
						animation_next(&(ship->animation));
					ship_render(ship,&data);
					game_send_data(g,&data,false);
					lista_next(g->enemies);
					break;
				case SHIP_END:
	//				printf(" Eliminamos: removemos de la lista\n");
					ship = lista_remove(g->enemies);
	//				printf(" Eliminamos: eliminamos nave\n");
					ship_destroy(ship);
					free(ship);
			}
		}
		/* Gestionamos disparos jugador */
	//	printf("game_playing_level(): Gestionamos disparos jugador\n");
		lista_first(g->shoot_player);
		while(!lista_eol(g->shoot_player)){
	//		printf("	Disparo jugador\n");
	//		printf("	Movemos\n");
			shoot_move(lista_get(g->shoot_player));
			/* Calculamos colision con enemigos */
			/* Esto quizas pueda eliminarse ya que se
		      realiza cuando se gestionan los enemigos */
	
	//		printf("	Colisiones con disparos\n");
			lista_first(g->shoot_player);
			while(!lista_eol(g->shoot_player)){
				ship_colision_shoot(	lista_get(g->enemies),
											lista_get(g->shoot_player));
				lista_next(g->shoot_player);
			}
	
			/* render info para cliente */
			lista_next(g->shoot_player);
		}
	
		/* Movemos disparos enemigos */
	//	printf("game_playing_level(): Gestionamos disparos enemigos\n");
		lista_first(g->shoot_enemies);
		while(!lista_eol(g->shoot_enemies)){
	//		printf("	Disparo enemigo\n");
	//		printf("	Movemos\n");
			shoot_move(lista_get(g->shoot_enemies));
			/* Calculamos colision con jugador */
	//		printf("	colision\n");
			ship_colision_shoot(g->player,lista_get(g->shoot_enemies));
			lista_next(g->shoot_enemies);
		}
	
		/* Movemos al jugador */
	//	printf("game_playing_level(): Movemos jugador\n");
		ship_move(g->player);
	//	printf("game_playing_level(): render jugador\n");
		ship_render(g->player,&data);
		game_send_data(g,&data,false);
	

	//	printf("game_playing_level(): buscamos cambios de estados level\n");
		switch(level_get_state(g->level)){
			case L_INGRESS:
	//			printf("	L_INGRESS\n");
				/* Si level esta en L_INGRESS, lo pasamos a L_PLAYING
					cuando hayan pasado 4 segundos de juego */
				if(clockgame_time(g->clock) >= 4)
					level_set_state(g->level,L_PLAYING);
				break;
			case L_PLAYING:
	//			printf("	L_PLAYING\n");
				/* Lanzamos nuevos enemigos si corresponde */
	//			printf("game_playing_level(): lanzamos enemigos\n");
				level_run(g->level,g->enemies);
				/* Si llegamos al final de la lista de ataques, no hay
					naves enemigas en pantalla o disparos enemigos,
					hemos terminado el nivel. La nave del jugador
					sale de pantalla */
				if(level_eol(g->level) && lista_size(g->enemies) == 0 &&
					lista_size(g->shoot_enemies) == 0){
						ship_remove_limits(g->player);
						ship_set_speed(g->player,10.0);
						ship_set_direction(g->player,0.0);
						level_set_state(g->level,L_EGRESS);
				}
				break;
			case L_EGRESS:
	//			printf("	L_EGRESS\n");
				/* Cuando la nave sale de la pantalla hemos terminado
					el nivel */
				if(point_get_x(ship_get_position(g->player)) >= SCREEN_WIDTH &&
					g->request_status == 0){
					level_set_state(g->level,L_END);
					/* colocamos en 1 request_status para que viaje
						el próximo udp con el bit encendido que avisa
						al cliente que requiere realizar un game_status */
					g->request_status = 1;
				}
				break;
			case L_END:
	//			printf("L_END\n");
				g->data.header.aux |= AUX_FORCESTATUS;
				
		}
		/* Enviamos los datos sobrantes en este loop */
		game_send_data(g,NULL,true);
		g->frame++;

		cantfotograms++;
		if(cantfotograms == FXS){
			clockgame_add(g->clock,1);
			cantfotograms=0;
		}
		nanosleep(&req,&rem);
	}
	printf("game_playing_level(): Salimos del bucle\n");
}

int game_get_state(game_t *g){
	return g->state;
}

void game_set_state(game_t *g, int state){
	sem_wait(&(g->sem_state));
		printf("Asignando GAME STATE:%i\n",state);
		g->state = state;
	sem_post(&(g->sem_state));
}

void game_info(game_t *g, game_info_t *info){
	g->request_status = 0;
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
