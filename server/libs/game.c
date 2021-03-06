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

	rect_set_point(&(g->limits),0,0);
	rect_set_dim(&(g->limits),SCREEN_WIDTH,SCREEN_HEIGHT);

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
	g->data.header.type = D_VIDEO;
	g->sound.header.size = 0;
	g->sound.header.type = D_SOUND;
	g->buffer = (char*)malloc(sizeof(char)*8);
}

void game_level_prepare(game_t *g){
	/* Daja preparado el juego para el nivel en cuestion */
	level_load(g->level,g->level_current, g->shoot_enemies);
	lista_clean(g->shoot_enemies,(void*)(void**)&shoot_destroy);
	lista_clean(g->shoot_player,(void*)(void**)&shoot_destroy);
	lista_clean(g->enemies,(void*)(void**)&ship_destroy);
	ship_set_direction(g->player,0);
	ship_set_speed(g->player,0);
	ship_set_position(g->player,100,300);
	ship_set_tangible(g->player,true);
	ship_reset_weapon(g->player);
	ship_activate_limits(g->player);

	g->direction.top = false;
	g->direction.bottom = false;
	g->direction.left = false;
	g->direction.top = false;

	g->frame = 0;
	clockgame_restore(g->clock);
}

void game_start(game_t *g){
	/* Resetea el juego y lo inicia en el nivel 1 */
	g->score = 0;
	g->level_current = 1;
	ship_set_state(g->player,SHIP_LIVE);
	ship_set_animation(g->player,0,1,false);
	ship_set_power(g->player,100);
	game_level_prepare(g);
	g->state=G_PLAYING;
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
						ship_shooting(g->player,true);
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
						ship_shooting(g->player,false);
						break;
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
		ship_set_direction(g->player,direction);
		ship_set_speed(g->player,module);
		free(g->events[i]);
	}
	g->event_size = 0;
	sem_post(g->sem_event);
}

void static game_send_data(game_t *g, data_render_t *data, bool at_once){
	/* Va armando el listado de datos a ser enviados por udp */
	/* Si at_once es true se envian inmediatamente. Sino se almacenan
		para ser enviados luego. Si no queda espacio de almacenamiento
		entonces los envia inmediatamente. */
	/* Data puede ser NULL si se desea enviar los datos ya almacenados
		y no se est?? sumando un dato nuevo (data == NULL)*/

	int n;

	if(data != NULL){
		data_entity_copy(&(g->data.body[g->data.header.size]),data);
		g->data.header.size ++;
	}

	if((at_once || g->data.header.size == MAX_DATA_BODY) &&
		g->data.header.size != 0){
		g->data.header.frame = g->frame;
		if(g->request_status)
			g->data.header.aux = AUX_FORCESTATUS;
		else
			g->data.header.aux = 0;

		data_to_buffer(&(g->data),&(g->buffer),&(g->buffer_size));
		n = sendto(g->sockfd, g->buffer, g->buffer_size, 0,
			(const struct sockaddr *) &(g->servaddr),sizeof(g->servaddr));
		g->data.header.size = 0;
	}
}

void static game_send_sound(game_t *g, int16_t *sound, bool at_once){
	int n;

	if(sound != NULL && *sound != -1){
		g->sound.sound[g->sound.header.size] = *sound;
		g->sound.header.size ++;
	}
	if((at_once || g->sound.header.size == MAX_DATA_BODY)
		&& g->sound.header.size != 0){
		g->sound.header.frame = g->frame;
		if(g->request_status)
			g->sound.header.aux = AUX_FORCESTATUS;
		else
			g->sound.header.aux = 0;
		data_to_buffer(&(g->sound),&(g->buffer),&(g->buffer_size));
		n = sendto(g->sockfd, g->buffer, g->buffer_size, 0,
			(const struct sockaddr *) &(g->servaddr),sizeof(g->servaddr));
		g->sound.header.size = 0;
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

void game_resume(game_t *g){
	game_set_state(g,G_PLAYING);
}

void static game_playing_level(game_t *g){
	/* Bucle que posee la logica del juego.
		GENERA un FRAME del juego  */

	struct timespec req;
	struct timespec rem;
	int cantfotograms;
	int wait;
	ship_t *ship;
	shoot_t *shoot;
	data_render_t data;
	int16_t sound;

	req.tv_sec = (1000/FXS) / 1000;
	req.tv_nsec = ((1000/FXS) % 1000) * 1000000;

	/* Leemos de un listado de acciones */

	g->data.header.size = 0;
	clockgame_start(g->clock);
	while(g->state == G_PLAYING){
		if(level_get_state(g->level) == L_PLAYING)
			game_handle_events(g);

		/* Gestionamos enemigos */
		lista_first(g->enemies);
		while(!lista_eol(g->enemies)){
			ship = lista_get(g->enemies);
			ship_go(ship);
			switch(ship_get_state(ship)){
				case SHIP_LIVE:
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

					/* Si sale de pantalla se elimina */
					/* La NAVE DEBE INICIAR CON AL MENOS UN PIXEL DENTRO
						DE LOS LIMITES. SINO SE AUTODESTRUYE */
					if(border_out_limits(ship_border(ship),&(g->limits))){
							ship_set_state(ship,SHIP_END);
					}

					ship_render(ship,&data,&sound);
					game_send_data(g,&data,false);
					game_send_sound(g,&sound,false);
					lista_next(g->enemies);
					break;
				case SHIP_DESTROY:
					if(animation_end(&(ship->animation)))
						ship_set_state(ship,SHIP_END);
					ship_render(ship,&data,&sound);
					game_send_data(g,&data,false);
					game_send_sound(g,&sound,false);
					lista_next(g->enemies);
					break;
				case SHIP_END:
					ship = lista_remove(g->enemies);
					ship_destroy(&ship);
			}
		}

		/* Gestionamos disparos del enemigo */
		lista_first(g->shoot_enemies);
		while(!lista_eol(g->shoot_enemies)){
			shoot = lista_get(g->shoot_enemies);
			switch(shoot_get_state(shoot)){
				case SHOOT_CREATED:
					shoot_set_state(shoot,SHOOT_LIVE);
				case SHOOT_LIVE:
					shoot_go(shoot);
					/* Calculamos colici??n con nave del jugador */
					ship = g->player;
					if(ship_colision_shoot(ship,shoot)){
						ship_set_power(ship, ship_get_power(ship) -
							shoot_get_damage(shoot));

						if(ship_get_power(ship) < 0){
							ship_begin_destroy(ship);
						}
						shoot_set_state(shoot,SHOOT_DESTROY);
					}

					/* Sale de pantalla? */
					if(border_out_limits(shoot_get_border(shoot),&(g->limits))){
						shoot_set_state(shoot,SHOOT_END);
					}

					shoot_render(shoot,&data,&sound);
					game_send_data(g,&data,false);
					lista_next(g->shoot_enemies);
					break;
				case SHOOT_DESTROY:
					if(animation_end(&(shoot->animation)))
						shoot_set_state(shoot,SHOOT_END);
					shoot_render(shoot,&data,&sound);
					game_send_data(g,&data,false);
					lista_next(g->shoot_enemies);
					break;
				case SHOOT_END:
					shoot = lista_remove(g->shoot_enemies);
					shoot_destroy(&shoot);
			}
		}

		/* Gestionamos disparos jugador */
		lista_first(g->shoot_player);
		while(!lista_eol(g->shoot_player)){
			shoot = lista_get(g->shoot_player);
			switch(shoot_get_state(shoot)){
				case SHOOT_CREATED:
					sound = SOUND_SHOOT1;
					game_send_sound(g,&sound,false);
					shoot_set_state(shoot,SHOOT_LIVE);
					break;
				case SHOOT_LIVE:
					shoot_go(shoot);
					/* Calculamos colici??n con naves enemigas */
					lista_first(g->enemies);
					while(!lista_eol(g->enemies)){
						ship = lista_get(g->enemies);
						if(ship_colision_shoot(ship,shoot)){
							ship_set_power(ship, ship_get_power(ship) -
								shoot_get_damage(shoot));
							if(ship_get_power(ship) < 0){
								ship_begin_destroy(ship);
								g->score += game_remunerate(ship_get_type(ship));
								g->request_status = 1;
		            		g->data.header.aux |= AUX_FORCESTATUS;
							}
							shoot_set_state(shoot,SHOOT_DESTROY);
						}
						lista_next(g->enemies);
					}

					/* Determinamos si sale de la pantalla */
					if(border_out_limits(shoot_get_border(shoot),&(g->limits))){
						shoot_set_state(shoot,SHOOT_END);
					}

					shoot_render(shoot,&data,&sound);
					game_send_data(g,&data,false);
					lista_next(g->shoot_player);
					break;
				case SHOOT_DESTROY:
					if(animation_end(&(shoot->animation)))
						shoot_set_state(shoot,SHOOT_END);
					else
						animation_next(&(shoot->animation));
					shoot_render(shoot,&data,&sound);
					game_send_data(g,&data,false);
					lista_next(g->shoot_player);
					break;
				case SHOOT_END:
					shoot = lista_remove(g->shoot_player);
					shoot_destroy(&shoot);
			}
		}

		/* Nave del jugador */
		ship = g->player;
		ship_go(ship);
		switch(ship_get_state(ship)){
			case SHIP_LIVE:
				ship_render(ship,&data,&sound);
				break;
			case SHIP_DESTROY:
				if(animation_end(&(ship->animation)))
					ship_set_state(ship,SHIP_END);
				ship_render(ship,&data,&sound);
				break;
			case SHIP_END:
				/* JUEGO FINALIZADO */
				g->request_status = 1;
				g->data.header.aux |= AUX_FORCESTATUS;
				ship_set_state(ship,SHIP_ZOMBI);
				level_set_state(g->level,L_GAME_OVER);
				wait = clockgame_time(g->clock) + 10;
				break;
			case SHIP_ZOMBI:
				break;
		}

		game_send_data(g,&data,false);
		game_send_sound(g,&sound,false);

		/* Estado del nivel que se esta jugando */
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
						ship_shooting(g->player,false);
						ship_set_direction(g->player,0.0);
						level_set_state(g->level,L_EGRESS);
				}
				break;
			case L_EGRESS:
				/* Avanzamos la nava automaticamente hasta que sale de la panalla */
				if(border_out_limits(ship_border(g->player),&(g->limits)) &&
					g->request_status == 0){
						if(g->level_current < CANT_LEVELS)
							/* Nivel siguiente */
							level_set_state(g->level,L_END);
						else
							/* No hay mas niveles */
							level_set_state(g->level,L_GAME_OVER);
						g->request_status = 1;
						g->data.header.aux |= AUX_FORCESTATUS;
						wait = clockgame_time(g->clock) + 10;  //1 segundos;
				}
				break;
			case L_GAME_OVER:
				/* El nivel ha finalizado porque el jugador ha
					perdido. Se esperan 4 segundos para pasar el
					juego a G_OVER. ??sto para dar tiempo a que el
					cliente genere un status */
				if (clockgame_time(g->clock) > wait)
					game_set_state(g,G_OVER);
				break;
			case L_END:
				/* El nivel ha finalizado */
				if (clockgame_time(g->clock) > wait){
					game_set_state(g,G_PAUSE);
					g->level_current++;
					game_level_prepare(g);
				}
		}
		/* Enviamos los datos sobrantes en este loop */
		game_send_data(g,NULL,true);
		game_send_sound(g,NULL,true);
		g->frame++;
		nanosleep(&req,&rem);
	}
	clockgame_stop(g->clock);
}

int game_get_state(game_t *g){
	return g->state;
}

void game_set_state(game_t *g, int state){
	sem_wait(&(g->sem_state));
		g->state = state;
	sem_post(&(g->sem_state));
}

void game_info(game_t *g, res_info_t *info){
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
