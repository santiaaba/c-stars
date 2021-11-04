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
	g->level_current = 1;

	g->enemies = (lista_t*)malloc(sizeof(lista_t));
	g->shoot_enemies = (lista_t*)malloc(sizeof(lista_t));
	g->shoot_player = (lista_t*)malloc(sizeof(lista_t));

	ship_init(g->player,SHIP_PLAYER, g->clock, g->shoot_player);
	ship_set_limits(g->player,SCREEN_HEIGHT,SCREEN_WIDTH);

	lista_init(g->enemies,sizeof(ship_t));
	lista_init(g->shoot_enemies,sizeof(shoot_t));
	lista_init(g->shoot_player,sizeof(shoot_t));

	clockgame_init(g->clock);

	g->data.header.size = 0;
	g->buffer = (char*)malloc(sizeof(char)*8);
}

void game_level_prepare(game_t *g){
	/* Daja preparado el juego para el nivel en cuestion */

	printf("game_level_start(): inicio level %i\n", g->level_current);
	level_load(g->level,g->level_current, g->shoot_enemies);
	lista_clean(g->shoot_enemies,(void*)(void**)&shoot_destroy);
	lista_clean(g->shoot_player,(void*)(void**)&shoot_destroy);
	lista_clean(g->enemies,(void*)(void**)&ship_destroy);
	ship_set_position(g->player,100,300);
	g->state = G_READY;
	g->frame = 0;
	clockgame_restore(g->clock);
}

void game_start(game_t *g){
	/* Resetea el juego y lo inicia en el nivel 1 */
	g->score = 0;
	g->level_current = 1;
	game_level_prepare(g);
	g->state=G_PLAYING;
}

void game_over(game_t *g){
	/* Finaliza el juego.
		Solo salir del bucle y no ingresar a el
		nuevamente salvo que se realice un game_start */
		game_set_state(g,G_OVER);
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
						ship_shooting(g->player,true);
						break;
					default:
						printf("game_handle_events(): KEY_DOWN Cualquier otra tecla\n");
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
						ship_shooting(g->player,false);
						break;
					default:
						printf("game_handle_events(): KEY_UP Cualquier otra tecla\n");
				}
		}
		if(g->direction.left || g->direction.right ||
		   g->direction.top || g->direction.bottom)
				module = PLAYER_MODULE;
		else
				module = 0;

		if(g->direction.left)
			direction = GRAD_180;
		if(g->direction.top)
			direction = GRAD_90;
		if(g->direction.right)
			direction = GRAD_0;
		if(g->direction.bottom)
			direction = GRAD_270;
		if(g->direction.left && g->direction.top)
			direction = GRAD_135;
		if(g->direction.right && g->direction.top)
			direction = GRAD_45;
		if(g->direction.left && g->direction.bottom)
			direction = GRAD_225;
		if(g->direction.right && g->direction.bottom)
			direction = GRAD_315;
		printf("game_handle_events(): (M,D) => (%f,%f)\n",module,direction);
		ship_set_direction(g->player,direction);
		ship_set_speed(g->player,module);
		//vector_set(vector,direction,module);
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
	g->state = G_READY;
	return 1;
}

int static game_remunerate(int ship_type){
	switch(ship_type){
		case SHIP_ENEMIE1: return 20;
	}
}

void static game_playing_level(game_t *g){
	/* Bucle que posee la logica del juego.
		GENERA un FRAME del juego  */

	struct timespec req;
	struct timespec rem;
	int cantfotograms;
	ship_t *ship;
	shoot_t *shoot;
	data_render_t data;

	req.tv_sec = (1000/FXS) / 1000;
	req.tv_nsec = ((1000/FXS) % 1000) * 1000000;

	/* Leemos de un listado de acciones */

	printf("game_playing_level(): Arrancamos el bucle\n");

	g->data.header.size = 0;
	clockgame_start(g->clock);
	while(g->state == G_PLAYING){
	//	continue;
		//printf("game_playing_level(): RELOJ: %"PRIu32"\n",clockgame_time(g->clock));
		if(level_get_state(g->level) == L_PLAYING)
			game_handle_events(g);
		/* Gestionamos enemigos */
		lista_first(g->enemies);
		while(!lista_eol(g->enemies)){
			ship = lista_get(g->enemies);
			//printf("ship(x,y):(%i,%i)\n",ship->position->x,ship->position->y);
			switch(ship_get_state(ship)){
				case SHIP_LIVE:
					ship_go(ship);
					/* Determinamos si debe comenzar a disparar */
					if(!ship_is_shooting(ship))
						if(point_get_x(ship_get_position(ship))<SCREEN_WIDTH)
							ship_shooting(ship,true);
					/* Calculamos colision con jugador */
					if(ship_colision_ship(ship,g->player)){
						/* Decrementamos energia jugador */
						ship_set_power(g->player,(ship_get_power(g->player) -
							ship_get_power(ship)));
						if(ship_get_power(g->player) <= 0){
							ship_begin_destroy(g->player);
						}
						/* Comenzamos eliminacion nave enemiga */
						ship_begin_destroy(ship);
					}
					ship_render(ship,&data);
					game_send_data(g,&data,false);
					lista_next(g->enemies);
					break;
				case SHIP_DESTROY:
					if(animation_end(&(ship->animation)))
						ship_set_state(ship,SHIP_END);
					else
						animation_next(&(ship->animation));
					ship_render(ship,&data);
					game_send_data(g,&data,false);
					lista_next(g->enemies);
					break;
				case SHIP_END:
					ship = lista_remove(g->enemies);
					ship_destroy(ship);
					free(ship);
			}
		}
		/* Gestionamos disparos del enemigo */
		lista_first(g->shoot_enemies);
		while(!lista_eol(g->shoot_enemies)){
			shoot = lista_get(g->shoot_enemies);
			switch(shoot_get_state(shoot)){
				case SHOOT_LIVE:
					//printf("shoot(x,y):(%i,%i)\n",shoot->position->x,shoot->position->y);
					shoot_go(shoot);
					/* Calculamos colición con nave del jugador */
					ship = g->player;
					if(ship_colision_shoot(ship,shoot)){
						//printf("COLISIONO DISPARO - power: %i - damage: %i = ",ship_get_power(ship),shoot_get_damage(shoot));
						ship_set_power(ship, ship_get_power(ship) -
							shoot_get_damage(shoot));
						//printf("power: %i\n",ship_get_power(ship));
						if(ship_get_power(ship) < 0){
							ship_begin_destroy(ship);
						}
						shoot_set_state(shoot,SHOOT_DESTROY);
					}
					shoot_render(shoot,&data);
					game_send_data(g,&data,false);
					lista_next(g->shoot_enemies);
					break;
				case SHOOT_DESTROY:
					if(animation_end(&(shoot->animation)))
						shoot_set_state(shoot,SHOOT_END);
					else
						animation_next(&(shoot->animation));
					shoot_render(shoot,&data);
					game_send_data(g,&data,false);
					lista_next(g->shoot_enemies);
					break;
				case SHOOT_END:
					shoot = lista_remove(g->shoot_enemies);
					shoot_destroy(shoot);
					free(shoot);
			}
		}

		/* Gestionamos disparos jugador */
		lista_first(g->shoot_player);
		while(!lista_eol(g->shoot_player)){
			shoot = lista_get(g->shoot_player);
			switch(shoot_get_state(shoot)){
				case SHOOT_LIVE:
					//printf("shoot(x,y):(%i,%i)\n",shoot->position->x,shoot->position->y);
					shoot_go(shoot);
					/* Calculamos colición con naves enemigas */
					lista_first(g->enemies);
					while(!lista_eol(g->enemies)){
						ship = lista_get(g->enemies);
						if(ship_colision_shoot(ship,shoot)){
							//printf("COLISIONO DISPARO - power: %i - damage: %i = ",ship_get_power(ship),shoot_get_damage(shoot));
							ship_set_power(ship, ship_get_power(ship) -
								shoot_get_damage(shoot));
							//printf("power: %i\n",ship_get_power(ship));
							if(ship_get_power(ship) < 0){
								g->score += game_remunerate(ship_get_type(ship));
								ship_set_state(ship,SHIP_DESTROY);
								ship_set_animation(ship,1,15,false);
								g->request_status = 1;
							}
							shoot_set_state(shoot,SHOOT_DESTROY);
						}
						lista_next(g->enemies);
					}
					shoot_render(shoot,&data);
					game_send_data(g,&data,false);
					lista_next(g->shoot_player);
					break;
				case SHOOT_DESTROY:
					if(animation_end(&(shoot->animation)))
						shoot_set_state(shoot,SHOOT_END);
					else
						animation_next(&(shoot->animation));
					shoot_render(shoot,&data);
					game_send_data(g,&data,false);
					lista_next(g->shoot_player);
					break;
				case SHOOT_END:
					shoot = lista_remove(g->shoot_player);
					shoot_destroy(shoot);
					free(shoot);
			}
		}
		/* Nave del jugador */
		ship_go(g->player);
		ship_render(g->player,&data);
		game_send_data(g,&data,false);
		switch(level_get_state(g->level)){
			case L_INGRESS:
				/* Si level esta en L_INGRESS, lo pasamos a L_PLAYING
					cuando hayan pasado 4 segundos de juego */
				if(clockgame_time(g->clock) >= 4)
					level_set_state(g->level,L_PLAYING);
				break;
			case L_PLAYING:
				/* Lanzamos nuevos enemigos si corresponde */
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
				/* Cuando la nave sale de la pantalla hemos terminado
					el nivel */
				if(point_get_x(ship_get_position(g->player)) >= SCREEN_WIDTH &&
					g->request_status == 0){
					level_set_state(g->level,L_END);
				}
				break;
			case L_END:
				/* El nivel ha finalizado */
				/* colocamos en 1 request_status para que viaje
					el próximo udp con el bit encendido que avisa
					al cliente que requiere realizar un game_status */
				g->request_status = 1;
				g->data.header.aux |= AUX_FORCESTATUS;

				if(g->level_current < CANT_LEVELS){
					printf("Pasamos al siguiente nivel\n");
					/* Hay un nivel siguiente */
					g->level_current++;
					game_level_prepare(g);
				} else {
					printf("No hay mas niveles\n");
					/* No hay mas niveles, terminamos el juego */
					game_set_state(g,G_OVER);
				}
				
		}
		/* Enviamos los datos sobrantes en este loop */
		game_send_data(g,NULL,true);
		g->frame++;

		nanosleep(&req,&rem);

	}
	printf("game_playing_level(): Salimos del bucle\n");
	clockgame_stop(g->clock);
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
	info->power = ship_get_power(g->player);
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
