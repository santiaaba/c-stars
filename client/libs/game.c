#include "game.h"

int static game_load_textures(game_t *g){
	/* Carga todas las texturas en el array */
	void load_entity(entitie_t *e, int w, int h, char *filename, SDL_Renderer *renderer){
		e->w = w;
		e->h = h;
		e->texture = IMG_LoadTexture(renderer, filename);
	}

	//Player1
	load_entity(&(g->entities[SHIP_PLAYER]),64,88,"img/player.png",g->renderer);
	if(g->entities[SHIP_PLAYER].texture == NULL)
		return 0;
	// Enemie 1
	load_entity(&(g->entities[SHIP_ENEMIE1]),133,138,"img/enemigo1.png",g->renderer);
	if(g->entities[SHIP_ENEMIE1].texture == NULL)
		return 0;
	//Shoot 1
	load_entity(&(g->entities[SHOOT_1]),14,14,"img/shoot1.png",g->renderer);
	if(g->entities[SHOOT_1].texture == NULL)
		return 0;

	/* Carga de los background */
	g->backgrounds[0] =IMG_LoadTexture(g->renderer, "img/background1.png");
}

int static game_load_sound(game_t *g){

	int i;

	//Explosion
	for(i=0;i<CANT_SOUNDS;i++){
		printf("ADASD\n");
		memset(&(g->sounds[i]), 0, sizeof(Mix_Chunk*) * 2);
		printf("ADASD___Adasd\n");
	}
	printf("Paso sonido\n");
	g->sounds[SOUND_EXPLOSION] = Mix_LoadWAV("sound/explosion.wav");
	g->sounds[SOUND_SHOOT1] = Mix_LoadWAV("sound/shoot1.wav");

	//FX_sounds
	for(i=0;i<CANT_SOUNDS;i++)
		memset(&(g->fx_sounds[i]), 0, sizeof(Mix_Chunk*) * 2);

	printf("Paso sonido\n");
	g->fx_sounds[FX_MENU] = Mix_LoadWAV("sound/menu.wav");
	printf("Paso sonido\n");
}

void static game_set_status(game_t *g, int status){
	sem_wait(&(g->sem_status));
		g->status = status;
	sem_post(&(g->sem_status));
}

void static close_connection(void *g){
	game_t *gg = (game_t*)g;
	req_t req;
	void server_response_handle(res_t *res){
		game_set_status(gg,DISCONNECTED);
		printf("Cerramos la conexión\n");
		tcp_client_disconnect(gg->command_cli);
	}
	req_init(&req);
	req_fill(&req,C_DISCONNECT,BODY_REQ_0);
	printf("Enviamos desconectarnos\n");
	tcp_client_send(gg->command_cli,&req,&server_response_handle);
	printf("Enviamos desconectarnos. Recibimos respuesta\n");
	req_destroy(&req);
}


int game_init(game_t *g){
	
	printf("Iniciando librería SDL\n");
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
		printf("No fue posible iniciar el video o audio.\n");
		printf("Error SDL: %s\n", SDL_GetError());
		return 0;
	}
	
	printf("Generando Ventana\n");
	g->window = SDL_CreateWindow(
		"C-Start", 20, 20, SCREEN_WIDTH,
		SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if(g->window == NULL){
		printf("No fue posible crear la ventana.\n");
		printf("Error SDL: %s\n", SDL_GetError());
		return 0;
	}

	printf("Levantando libreria de fuentes\n");
	if(TTF_Init() == -1){
		printf("Error al inicializar la librería TTF\n");
		return 0;
	}

	g->status_in_progress = false;
	g->renderer = SDL_CreateRenderer(g->window, -1, 0);
	g->status = HELLO;
	g->udp = 20000;	// Lo fijamos por ahora
	g->data.header.size = 0;

	printf("Iniciando semáforo sem_status\n");
	if(sem_init(&(g->sem_status),0,1) == -1){
		fprintf(stderr, "game_init() failed: %s\n", strerror(errno));
		return 0;
	}

	printf("Iniciando semáforo sem_data\n");
	if(sem_init(&(g->sem_data),0,1) == -1){
		fprintf(stderr, "game_init() failed: %s\n", strerror(errno));
		return 0;
	}

	printf("Iniciando sonido");
	if(Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512) < 0){
		fprintf(stderr, "Error al iniciar el audio: %s\n", SDL_GetError());
		return 0;
	}
	if(Mix_AllocateChannels(4)<0){
		fprintf(stderr, "Unable to allocate mixing channels: %s\n", SDL_GetError());
		return 0;
	}

	printf("Creando cliente de comandos\n");
	g->command_cli = (tcp_client_t *)malloc(sizeof(tcp_client_t));

	printf("Cargando texturas\n");
	game_load_textures(g);

	printf("Cargando sonidos\n");
	game_load_sound(g);

	return 1;
}

int game_check_connect(){
	/* Verifica que la conexion al server todavia
		se encuentre activa */
}

void static game_status(game_t *g){
	/* Solicita al servidor el estado del juego */
	req_t req;
	res_t res;

	void server_response_handle(res_t *res){
		sem_wait(&(g->sem_status));
			g->playing_state = ((res_info_t*)(res->body))->state;
			g->score = ((res_info_t*)(res->body))->score;
			g->power_ship = ((res_info_t*)(res->body))->power;
			g->level = ((res_info_t*)(res->body))->level;
			g->level_state = ((res_info_t*)(res->body))->level_state;
			g->status_in_progress = false;
		sem_post(&(g->sem_status));
	}

	req_init(&req);
	req_fill(&req,C_GAME_STATUS,BODY_REQ_0);
	tcp_client_send(g->command_cli,&req,&server_response_handle);
}

void *game_data_recive(game_t *g){
	/* Queda en un hilo esperando que ingresen datos nuevos */
	int len, n;
	char buffer[MAX_DATA];

	len = sizeof(g->cliaddr);

	while(g->status != DISCONNECTED){
		n = recvfrom(g->sockfd, (char *)buffer, MAX_DATA, 
				MSG_WAITALL, ( struct sockaddr *) &(g->cliaddr),
				&len);
		printf("Datos recibidos: %i\n",n);
		if (n < 0)
			close_connection(g);
		
		sem_wait(&(g->sem_data));
			buffer_to_data(&(g->data),buffer);
		sem_post(&(g->sem_data));
	}
}

void static game_render(game_t *g){

	SDL_Rect position;
	char score[100];
	SDL_Rect frame;
	Mix_Chunk *sound01 = NULL;
	int index_entity;
	text_t text_score;
	text_t text_energia;
	powerbar_t powerbar;
	background_t bg;


	SDL_RenderClear(g->renderer);
	SDL_RenderPresent(g->renderer);
	game_status(g);
	g->screen_frame = 0;
	text_init(&text_score,800,30,16,g->renderer);
	text_init(&text_energia,200,20,16,g->renderer);
	text_set(&text_energia,"Energia:");

	background_init(&bg,g->renderer,g->backgrounds[g->level - 1]);
	//background_begin(&bg);

	powerbar_init(&powerbar,g->renderer);
	powerbar_set_position(&powerbar,259,22);
	powerbar_set_max(&powerbar,100);
	powerbar_set_power(&powerbar,g->power_ship);
	int l = 0;
	while(g->status == PLAYING){

		sem_wait(&(g->sem_data));
		if(g->data.header.size == 0){
			/* Hacer nada. Esperar unos milisegundos */
			printf("Esperamos\n");
			SDL_Delay(10);
		} else {
			if(g->data.header.type == D_VIDEO){
			/* Es D_VIDEO */
				if(g->data.header.aux && AUX_FORCESTATUS && g->status_in_progress == false){
					sem_wait(&(g->sem_status));
					g->status_in_progress = true;
					sem_post(&(g->sem_status));
					/* El server nos solicita consultar por el estado */
					pthread_create(&(g->th_status), NULL, (void*)(void*)(&game_status),g);
				}
		
				/* Si la data corresponde a un frame nuevo entonces
					dibujamos la pantalla. */
				if(g->data.header.frame != g->screen_frame){
					g->screen_frame = g->data.header.frame;
		
					text_set(&text_score,score);
					text_draw(&text_score);
					text_draw(&text_energia);
					powerbar_set_power(&powerbar,g->power_ship);
					powerbar_draw(&powerbar);
					SDL_RenderPresent(g->renderer);
					printf("Nuevo frame: %i\n",l);
					l++;
					SDL_RenderClear(g->renderer);
					background_draw(&bg);
				}
				for(int i=0;i<g->data.header.size;i++){
					index_entity = g->data.body[i].entity_class;
//					printf("Dibujando entidad en index: %i\n",index_entity);
					if(index_entity != -1){
						/* Rectangulo para dibujar en pantalla */
						position.x = g->data.body[i].pos_x;
						position.y = g->data.body[i].pos_y;
						position.w = g->entities[index_entity].w;
						position.h = g->entities[index_entity].h;
//						printf("Dibujando entidad: position:(x,y,w,h) = (%i,%i,%i,%i)\n",
//								position.x,position.y,position.w,position.h);
			
						/* Rectangulo para recortar la textura */
						frame.w = position.w;
						frame.h = position.h;
						frame.y = frame.h * g->data.body[i].sprite;
						frame.x = frame.w * g->data.body[i].frame;
//						printf("Dibujando entidad: recorte:(x,y,w,h) = (%i,%i,%i,%i)\n",
//								frame.x,frame.y,frame.w,frame.h);
			
						/* Dibujamos */
						SDL_RenderCopy(
							g->renderer,
							g->entities[index_entity].texture,
							&frame,
							&position);
					}
				}
			} else {
				/* Es D-SOUND */
				for(int i=0;i < g->data.header.size;i++){
					Mix_PlayChannel(-1, g->sounds[g->data.sound[i]], 0);
					printf("Sonido: %i\n",g->data.sound[i]);
				}
			}
			g->data.header.size = 0;
		}
		printf("game_render(): PEGA LA VUELTA:%u\n",g->status);
		sem_post(&(g->sem_data));
		SDL_Delay(10);
	}
	printf("game_render(): SALIMOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOSSSS:%u\n",g->status);
	powerbar_destroy(&powerbar);
	return;
}

int static game_start_udp_server(game_t *g){
	/* Inicia el server UDP que recibe los datos
		para confeccionar las pantallas y los sonidos. */

	if ( (g->sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		return 0;
	}

	memset(&(g->servaddr), 0, sizeof(g->servaddr));
	memset(&(g->cliaddr), 0, sizeof(g->cliaddr));

	g->servaddr.sin_family	= AF_INET; // IPv4
	g->servaddr.sin_addr.s_addr = INADDR_ANY;
	g->servaddr.sin_port = htons(g->udp);

	if ( bind(g->sockfd, (const struct sockaddr *)&(g->servaddr),
		sizeof(g->servaddr)) < 0 ){
			perror("bind failed");
			return 0;
	}
	printf("Server UDP iniciado\n");
	return 1;
}

void game_hello(game_t *g){
	/* Pantalla de presentacion. En esta pantalla el
		juego aún no está conectado con el servidor.
		Se aguarda precionar cualquier tecla para pasar
		a la pantalla de conexion */
	SDL_Event event;
	SDL_Rect bg_frame, bg_dest;
	SDL_Texture *bg_texture;
	text_t label1;

	bg_frame.x = 0;
	bg_frame.y = 0;
	bg_frame.h = SCREEN_HEIGHT;
	bg_frame.w = SCREEN_WIDTH;

	bg_dest.x = 0;
	bg_dest.y = 0;
	bg_dest.h = SCREEN_HEIGHT;
	bg_dest.w = SCREEN_WIDTH;

	bg_texture = IMG_LoadTexture(g->renderer, "img/hello_bg.jpg");

	text_init(&label1,200,200,60,g->renderer);
	text_set(&label1,"C-STAR");
	while(g->status == HELLO){
		/* Si presionamos cualquier tecla, pasamos la pantalla */
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_KEYDOWN)
				g->status = DISCONNECTED;
		}
		/* Borramos la pantalla */
		SDL_RenderClear(g->renderer);
		/* Pintamos el background */
		SDL_RenderCopy(g->renderer, bg_texture, &bg_frame, &bg_dest);
		text_draw(&label1);
		/* Le pedimos al screen que renderise */
		SDL_RenderPresent(g->renderer);

		/* Movemos el background */
		if(bg_frame.x < 200){
			bg_frame.x++;
		}
		SDL_Delay(SCREEN_REFRESH);
	}
}

void game_play(game_t *g){
	/* En este modo, el juego esta corriendo. Se deben dibujar
		todos los frames en base a lo que se encuentra en el buffer.
		Se deben capturar los eventos del teclado y enviar los mismos
		al servidor del juego. */

	typedef struct {
		uint16_t up;
		uint16_t down;
		uint16_t left;
		uint16_t right;
		uint16_t space;
	} key_last_state_t;

	bool new_event;
	SDL_Event event;
	SDL_Rect frame;
	SDL_Rect position;
	int i,j;
	char *buffer_req;
	int buffer_req_size;
	data_t *data;
	req_t req;
	key_last_state_t key_last_state;

	/* Pausar juego */
	void pause_game(game_t *g){
		void server_response_handle(res_t *res){
			if(res->header.resp == RES_OK){
				printf("Comenzo el siguiente nivel\n");
				game_set_status(g,PAUSE);
			}
		}
		req_t req;
		req_init(&req);
		req_fill(&req,C_GAME_PAUSE,BODY_REQ_0);
	//	printf("Enviamos pausar\n");
		tcp_client_send(g->command_cli,&req,&server_response_handle);
		req_destroy(&req);
	}

	key_last_state.up = SDL_KEYUP;
	key_last_state.down = SDL_KEYUP;
	key_last_state.left = SDL_KEYUP;
	key_last_state.right = SDL_KEYUP;
	key_last_state.space = SDL_KEYUP;

	req_init(&req);
	req_fill(&req,C_KEY_PRESS,BODY_REQ_KP); //4
	req.body = (req_kp_t*)malloc(sizeof(req_kp_t));
	g->screen_frame = 0;
	while(g->status == PLAYING){
		/* Capturamos los eventos del teclado y los colocamos en el
			buffer compartido con el hilo del cliente de comandos TCP */
		//printf("Capturamos y enviamos eventos\n");
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_KEYUP &&
				event.key.keysym.sym == SDLK_ESCAPE){
					pause_game(g);
					break;
			}
			if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) &&
				(event.key.keysym.sym == SDLK_LEFT ||
				 event.key.keysym.sym == SDLK_RIGHT ||
			 	 event.key.keysym.sym == SDLK_UP ||
				 event.key.keysym.sym == SDLK_DOWN ||
				 event.key.keysym.sym == SDLK_SPACE )){

				new_event = false;
				switch(event.key.keysym.sym){
					case SDLK_LEFT:
						if(key_last_state.left != event.type){
							new_event = true;
							key_last_state.left = event.type;
						}
						break;
					case SDLK_RIGHT:
						if(key_last_state.right != event.type){
							new_event = true;
							key_last_state.right = event.type;
						}
						break;
					case SDLK_UP:
						if(key_last_state.up != event.type){
							new_event = true;
							key_last_state.up = event.type;
						}
						break;
					case SDLK_DOWN:
						if(key_last_state.down != event.type){
							new_event = true;
							key_last_state.down = event.type;
						}
						break;
					case SDLK_SPACE:
						if(key_last_state.space != event.type){
							new_event = true;
							key_last_state.space = event.type;
						}
				}

				if(new_event){
					((req_kp_t*)(req.body))->key = event.key.keysym.sym;
					((req_kp_t*)(req.body))->action = event.type;
					printf("Enviamos evento-->\n");
					tcp_client_send(g->command_cli,&req,NULL);
				}
			}
		}

		SDL_Delay(SCREEN_REFRESH);

		/* Controlamos el estado del play y del nivel */
		if(g->playing_state == G_OVER){
			printf("Cambiamos de estdo a END_GAME\n");
			game_set_status(g,END_GAME);
		} else {
			if(g->level_state == L_END){
				printf("Cambiamos de estdo a END_LEVEL\n");
				game_set_status(g,END_LEVEL);
			}
		}
	}
	return;
}

void game_connect(game_t *g){
	/* En este modo, el juego presenta una pantalla
		para ingresar la ip del servidor e intentar
		conectar al mismo. No se cambia de estado si
		no se logra la conexión. Presionando ESC
		se regresa a la pantalla hello */

	input_t ip_server;
	text_t error,salir;
	pthread_t th;		// Para indicarle al server iniciar el juego
	int key = 0;
	SDL_Event event;
	int key_press = 0;
	int wait = false;
	char *srv_ip;
	char message[200];
	char borrar[1000];
	bool end = false;
	SDL_Texture *background;
	SDL_Rect bg_rect;

	void *try_to_connect(void *g){
		game_t *gg = (game_t*)g;
		req_t req;
		char *buffer_req = NULL;
		int buffer_req_size = 0;

		void server_response_handle(res_t *res){
			printf("Manejando la respuesta del server\n");
			if(res->header.resp == RES_OK){
				game_start_udp_server(gg);
				game_set_status(gg,CONNECTED);

				/* Iniciamos el hilo que deja al server udp
					recibiendo datos */
				pthread_create(&(gg->th_recive),NULL,(void*)(void*)(&game_data_recive),gg);
				end = true;
				req_fill(&req,C_CONNECT_2,BODY_REQ_0);
				tcp_client_send(gg->command_cli,&req,NULL);
			} else {
				switch(res->header.resp){
					case RES_ERROR_PORT:
						strcat(message,"Puerto UDP no aceptado");
						break;
					case RES_ERROR_VERSION:
						strcat(message,res->body);
						break;
					default:
						strcat(message,"Error indefinido");
				}
				/* Enviando rechazo en paquete req que no espera
					respuesta */
			}
			wait = false;
		}

		if(!tcp_client_init(gg->command_cli,srv_ip,SRV_PORT)){
			text_set(&error,"Error fatal al crear el socket");
			fprintf(stderr, "game_connect() failed: %s\n", strerror(errno));
			wait = false;
			return NULL;
		}
		
		printf("Conectando contra el servidor\n");
		if(!tcp_client_connect(gg->command_cli)){
			text_set(&error,"Server no responde");
			fprintf(stderr, "game_connect() failed: %s\n", strerror(errno));
			wait = false;
			return NULL;
		}
		printf("Enviando udp y version\n");
		req_init(&req);
		req_fill(&req,C_CONNECT_1,BODY_REQ_CONNECT); //sizeof(req_connect_t)
		req.body = (req_connect_t*)malloc(sizeof(req_connect_t));

		((req_connect_t*)(req.body))->udp = gg->udp;
		((req_connect_t*)(req.body))->version = EAEAPP_VERSION;

		tcp_client_send(gg->command_cli,&req,&server_response_handle);
		req_destroy(&req);
		return NULL;
	}

	input_init(&ip_server,250,200,1,g->renderer);
	text_init(&error,300,300,25,g->renderer);
	text_init(&salir,300,500,25,g->renderer);
	text_set(&salir,"Presionar Esc para regresar");
	background = IMG_LoadTexture(g->renderer, "img/bg_menu.png");
	bg_rect.x = 0;
	bg_rect.y = 0;
	bg_rect.w = 1024;
	bg_rect.h = 600;

	while(!end){
		/* Borramos la pantalla */
		SDL_RenderClear(g->renderer);
		SDL_RenderCopy(
			g->renderer,
			background,
			&bg_rect,
			&bg_rect);
		if(!wait){
			while(SDL_PollEvent(&event)){
				/* Solo aceptamos los numeros, el punto y el backspace */
				if(event.type == SDL_KEYDOWN){
					key_press = 1;
					key = event.key.keysym.sym;
					if ((key >= SDLK_0 && key <= SDLK_9) || key == SDLK_PERIOD){
							input_add_char(&ip_server,key);
					}
					if (key == SDLK_BACKSPACE){
						input_del_char(&ip_server);
						printf("backspace\n");
					}
					if (key == SDLK_ESCAPE)
						end = true;
					if (key == SDLK_RETURN){
						wait = true;
						srv_ip = input_get_value(&ip_server);
						printf("Creando el hilo\n");
						text_set(&error,"Tratando de conectar");
						pthread_create(&th,NULL,&try_to_connect,g);
					}
				} else {
					if(event.type == SDL_KEYUP)
						key_press = 0;
				}
			}
			/* render del input */
			input_draw(&ip_server);
			text_draw(&error);
			text_draw(&salir);
		} else {
			/* Estamos aguardando respuesta del server */
			/* Dibujar algo que represente la espera */
		}
		/* Render pantalla */
		SDL_RenderPresent(g->renderer);
		SDL_Delay(SCREEN_REFRESH);
	}
}

void static game_end_level(game_t *g){

	SDL_Event event;
	int key = 0;
	text_t text_endGame;
	text_t text_pressEnter;
	req_t req;

	void server_response_handle(res_t *res){
		if(res->header.resp == RES_OK){
			printf("Comenzo el siguiente nivel\n");
			game_set_status(g,PLAYING);
		}
	}

	text_init(&text_endGame,300,300,25,g->renderer);
	text_init(&text_pressEnter,300,900,25,g->renderer);

	text_set(&text_endGame,"Nivel Finalizado");
	text_set(&text_pressEnter,"Presione ENTER para siguiente nievel");

	while(g->status == END_LEVEL){
		SDL_RenderClear(g->renderer);
		while(SDL_PollEvent(&event)){
			/* Esperamos se presione enter */
			if(event.type == SDL_KEYDOWN){
				key = event.key.keysym.sym;
				if (key == SDLK_RETURN){
					printf("Pasamos al siguiente nivel\n");
					req_init(&req);

					req_fill(&req,C_GAME_RESUME,BODY_REQ_0);
					tcp_client_send(g->command_cli,&req,&server_response_handle);
					req_destroy(&req);
				}
			}
		}
		text_draw(&text_endGame);
		text_draw(&text_pressEnter);
		SDL_RenderPresent(g->renderer);
		SDL_Delay(SCREEN_REFRESH);
	}
}

void static game_end_game(game_t *g){

	SDL_Event event;
	int key = 0;
	text_t text_endGame;
	text_t text_pressEnter;

	text_init(&text_endGame,300,200,50,g->renderer);
	text_init(&text_pressEnter,300,400,20,g->renderer);

	text_set(&text_endGame,"Juego Finalizado");
	text_set(&text_pressEnter,"Presione ENTER para continuar");

	while(g->status == END_GAME){
		SDL_RenderClear(g->renderer);
		while(SDL_PollEvent(&event)){
			/* Esperamos se presione enter */
			if(event.type == SDL_KEYDOWN){
				key = event.key.keysym.sym;
				if (key == SDLK_RETURN){
					printf("Pasamos al menu principal\n");
					game_set_status(g,CONNECTED);
				}
			}
		}
		text_draw(&text_endGame);
		text_draw(&text_pressEnter);
		SDL_RenderPresent(g->renderer);
		SDL_Delay(SCREEN_REFRESH);
	}
}

void game_main_menu(game_t *g){
	/* Loop del menu principal. Las opciones del menu son:
		- Jugar
		- Creditos
		- Desconectar
	*/
	menu_t menu;
	int key;
	SDL_Event event;
	bool pusshed;
	pthread_t th;
	SDL_Texture *background;
	SDL_Rect bg_rect;

	void *pre_start_game(void *g){
		/* Encargado de enviar al servidor el mensaje de
			iniciar el juego en el nivel 1 */
		game_t *gg = (game_t*)g;
		req_t req;

		void server_response_handle(res_t *res){
			if(res->header.resp == RES_OK){
				printf("Comenzo el juego\n");
				game_set_status(gg,PLAYING);
			} else {
				//text_set(&message,"Error");
				menu_unlooked(&menu);
				menu_show(&menu);
			}
		}

		/* Enviar mensaje para iniciar el juego */
		req_init(&req);
		req_fill(&req,C_GAME_START,BODY_REQ_0);
		tcp_client_send(gg->command_cli,&req,&server_response_handle);
		req_destroy(&req);
	}

	void make_mainMenu(menu_t *menu, int status){
		/* Encargado de armar el menu a mostrar
			en pantalla */
		printf("Reamando el menu");
		menu_destroy(menu);
		switch(status){
			case DISCONNECTED:
				menu_add(menu,"Conectar");
				menu_add(menu,"Creditos");
				menu_add(menu,"Salir");
				break;
			case CONNECTED:
				menu_add(menu,"Jugar");
				menu_add(menu,"Desconectar");
				menu_add(menu,"Creditos");
				menu_add(menu,"Salir");
				break;
		}
	}

	printf("--- MAIN-MENU ---\n");
	menu_init(&menu,300,100,g->renderer);
	make_mainMenu(&menu,g->status);
	pusshed = false;
	background = IMG_LoadTexture(g->renderer, "img/bg_menu.png");
	bg_rect.x = 0;
	bg_rect.y = 0;
	bg_rect.w = 1024;
	bg_rect.h = 600;
	//text_init(&message,200,200,25,g->renderer);
	while(g->status == DISCONNECTED || g->status == CONNECTED){
		while(SDL_PollEvent(&event)){
			if(menu_is_looked(&menu))
				break;
			if(!pusshed && event.type == SDL_KEYDOWN){
				pusshed=true;
				/* Solo aceptamos las flechas arriba, abajo y enter */
				key = event.key.keysym.sym;
				switch(key){
					case SDLK_DOWN:
						menu_down(&menu);
						Mix_PlayChannel(-1, g->fx_sounds[FX_MENU], 0);
						break;
					case SDLK_UP:
						menu_up(&menu);
						Mix_PlayChannel(-1, g->fx_sounds[FX_MENU], 0);
						break;
					case SDLK_RETURN:
						switch(g->status){
							case DISCONNECTED:
								switch(menu_get_index(&menu)){
									case 0:
										/* CONECTAR */
										game_connect(g);
										if(g->status == CONNECTED)
											make_mainMenu(&menu,g->status);
										break;
									case 1:
										/* CREDITOS */
										printf("Implementar creditos");
										break;
									case 2:
										/* SALIR */
										game_set_status(g,END);
								}
								break;
							case CONNECTED:
								switch(menu_get_index(&menu)){
									case 0:
										/* JUGAR */
										pre_start_game(g);
										break;
									case 1:
										/* DESCONECTAR */
										close_connection(g);
										if(g->status == DISCONNECTED)
											make_mainMenu(&menu,g->status);
										break;
									case 2:
										/* CREDITOS */
										printf("Implementar creditos");
										break;
									case 3:
										/* SALIR */
										close_connection(g);
										game_set_status(g,END);
								}
							}
					}
			}
			if(pusshed && event.type == SDL_KEYUP)
				pusshed=false;
		}	// while SDL_PoolEvents
		SDL_RenderClear(g->renderer);
		SDL_RenderCopy(
			g->renderer,
			background,
			&bg_rect,
			&bg_rect);
		menu_draw(&menu);
		SDL_RenderPresent(g->renderer);
		SDL_Delay(SCREEN_REFRESH);
	} // while g_satus
	menu_destroy(&menu);
}

void game_pause(game_t *g){
	/* Loop cuando se esta presentando la pausa */

	menu_t menu;
	int key;
	SDL_Event event;
	bool pusshed;

	/* Continuar juego */
	void resume_game(game_t *g){
		req_t req;
		void server_response_handle(res_t *res){
			game_set_status(g,PLAYING);
		}
		req_init(&req);
		req_fill(&req,C_GAME_RESUME,BODY_REQ_0);
		printf("Enviamos continuar\n");
		tcp_client_send(g->command_cli,&req,&server_response_handle);
		req_destroy(&req);
	}

	/* Terminar juego */
	void end_game(game_t *g){
		req_t req;
		void server_response_handle(res_t *res){
			game_set_status(g,CONNECTED);
		}
		req_init(&req);
		req_fill(&req,C_GAME_STOP,BODY_REQ_0);
		printf("Enviamos terminar\n");
		tcp_client_send(g->command_cli,&req,&server_response_handle);
		req_destroy(&req);
	}

	printf("--- PAUSE ---\n");
	menu_init(&menu,100,100,g->renderer);
	menu_add(&menu,"Continuar");
	menu_add(&menu,"Terminar");

	pusshed = false;
	while(g->status == PAUSE){
		while(SDL_PollEvent(&event)){
			if(menu_is_looked(&menu))
				break;
			if(!pusshed && event.type == SDL_KEYDOWN){
				pusshed=true;
				/* Solo aceptamos las flechas arriba, abajo y enter */
				key = event.key.keysym.sym;
				switch(key){
					case SDLK_DOWN:
						menu_down(&menu);
						break;
					case SDLK_UP:
						menu_up(&menu);
						break;
					case SDLK_RETURN:
						switch(menu_get_index(&menu)){
							case 0:
								/* Impedimos los eventos */
								menu_looked(&menu);
								resume_game(g);
								break;
							case 1:
								menu_looked(&menu);
								end_game(g);
								break;
						}
				}
			}
			if(pusshed && event.type == SDL_KEYUP)
				pusshed=false;
		}
		SDL_RenderClear(g->renderer);
		menu_draw(&menu);
		SDL_RenderPresent(g->renderer);
		SDL_Delay(SCREEN_REFRESH);
	}
	menu_destroy;
}

void *game_run(game_t *g){
	pthread_t th_render;
	//pthread_t th_playing;

	printf("Arrancamos GAME\n");
	while(g->status != END){
		switch(g->status){
			case HELLO:
				game_hello(g);
				break;
			case PLAYING:
				pthread_create(&th_render, NULL, (void*)(void*)(&game_render),g);
				game_play(g);
				break;
			case DISCONNECTED:
			case CONNECTED:
				game_main_menu(g);
				break;
			case END_LEVEL:
				game_end_level(g);
				break;
			case END_GAME:
				game_end_game(g);
				break;
			case PAUSE:
				game_pause(g);
				break;
		}
	}
}

void game_free(game_t *g){
	SDL_Quit();
}
