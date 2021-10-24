#include "game.h"

void static game_load_textures(game_t *g){
	/* Carga todas las texturas en el array */

	//Player1
	g->entities[0].w = 64;
	g->entities[0].h = 88;
	g->entities[0].texture = IMG_LoadTexture(g->renderer, "img/player.png");

	// Enemie 1
	g->entities[1].w = 133;
	g->entities[1].h = 138;
	g->entities[1].texture = IMG_LoadTexture(g->renderer, "img/enemigo1.png");
}

int game_init(game_t *g){
	
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("No fue posible iniciar el video.\n");
		printf("Error SDL: %s\n", SDL_GetError());
		return 1;
	}

	g->window = SDL_CreateWindow(
		"C-Start", 20, 20, SCREEN_WIDTH,
		SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if(g->window == NULL){
		printf("No fue posible crear la ventana.\n");
		printf("Error SDL: %s\n", SDL_GetError());
		return 1;
	}

	if(TTF_Init() == -1){
		printf("Error al inicializar la librería TTF\n");
		return 1;
	}

	g->renderer = SDL_CreateRenderer(g->window, -1, 0);
	g->status = HELLO;
	g->udp = 20000;	// Lo fijamos por ahora
	if(sem_init(&(g->sem_status),0,1) == -1){
		fprintf(stderr, "game_init() failed: %s\n", strerror(errno));
	}
	g->command_cli = (tcp_client_t *)malloc(sizeof(tcp_client_t));

	game_load_textures(g);
}

int static game_map_texture(uint16_t idEntity){
	switch(idEntity){
		case 20: return 0;	// Player
		case 21: return 1;	// Enemie 1
		case 22: return -1;	// Enemie 2
		case 23: return -1;	// Enemie 3

		case 0: return -1;		// Shoot 1
		case 1: return -1;		// Shoot 2
		case 2: return -1;		// Shoot 3
		default: return -1;
	}
}

int game_check_connect(){
	/* Verifica que la conexion al server todavia
		se encuentre activa */
}

void static game_render(game_t *g){

	int len, n;
	data_t data;
	char buffer[MAX_DATA];
	SDL_Rect position;
	SDL_Rect frame;
	int index_entity;

	len = sizeof(g->cliaddr);

	SDL_RenderClear(g->renderer);
	SDL_RenderPresent(g->renderer);
	printf("game_render(): Entro render: %u\n",g->status);
	g->screen_frame = 0;
	while(g->status == PLAYING){
		printf("game_render(): Esperando datos de render\n");
		n = recvfrom(g->sockfd, (char *)buffer, MAX_DATA, 
				MSG_WAITALL, ( struct sockaddr *) &(g->cliaddr),
				&len);
		printf("game_render(): Datos de render recibidos: %i\n",n);
 
		buffer_to_data(&data,buffer);
		/* Si la data corresponde a un frame nuevo entonces
			dibujamos la pantalla. */
		if(data.header.frame != g->screen_frame){
			g->screen_frame = data.header.frame;
			SDL_RenderPresent(g->renderer);
			printf("Nuevo frame\n");
			SDL_RenderClear(g->renderer);
		}
		for(int i=0;i<data.header.size;i++){
			index_entity = game_map_texture(data.body[i].entity_class);
			printf("Dibujando entidad en index: %i\n",index_entity);
			if(index_entity != -1){
				/* Rectangulo para dibujar en pantalla */
				position.x = data.body[i].pos_x;
				position.y = data.body[i].pos_y;
				position.w = g->entities[index_entity].w;
				position.h = g->entities[index_entity].h;
				printf("Dibujando entidad: position:(x,y,w,h) = (%i,%i,%i,%i)\n",
						position.x,position.y,position.w,position.h);
	
				/* Rectangulo para recortar la textura */
				frame.w = position.w;
				frame.h = position.h;
				frame.y = frame.h * data.body[i].sprite;
				frame.x = frame.w * data.body[i].frame;
				printf("Dibujando entidad: recorte:(x,y,w,h) = (%i,%i,%i,%i)\n",
						frame.x,frame.y,frame.w,frame.h);
	
				/* Dibujamos */
				SDL_RenderCopy(
					g->renderer,
					g->entities[index_entity].texture,
					&frame,
					&position);
			}
		}
	}
	printf("game_render(): Salio render:%u\n",g->status);
}


void static game_set_status(game_t *g, int status){
	sem_wait(&(g->sem_status));
		g->status = status;
	sem_post(&(g->sem_status));
}

int static game_start_udp_server(game_t *g){
	/* Inicia en thread que se encarga de recibir los
		paquetes de UDP con las instrucciones para
		el dibujado de la pantalla */

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

void *game_run(game_t *g){
	printf("Arrancamos GAME\n");
	while(g->status != END){
		switch(g->status){
			case HELLO:
				game_hello(g);
				break;
			case PLAYING:
				pthread_create(&(g->th_render), NULL, (void*)(void*)(&game_render),g);
				game_play(g);
				break;
			case DISCONNECTED:
			case CONNECTED:
				game_main_menu(g);
				break;
			case PAUSE:
				game_pause(g);
				break;
		}
	}
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

	text_t debug;
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
		req_t req;
		req_init(&req);
		req_fill(&req,C_GAME_PAUSE,0);
		printf("Enviamos pausar\n");
		tcp_client_send(g->command_cli,&req,NULL);
		game_set_status(g,PAUSE);
		req_destroy(&req);
	}

	key_last_state.up = SDL_KEYUP;
	key_last_state.down = SDL_KEYUP;
	key_last_state.left = SDL_KEYUP;
	key_last_state.right = SDL_KEYUP;
	key_last_state.space = SDL_KEYUP;

	text_init(&debug,300,300,25,g->renderer);
	text_set(&debug,"Jugando");

	req_init(&req);
	req_fill(&req,C_KEY_PRESS,4);
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

		/* Dibutajos la pantalla si es que tenemos informacion
			en el buffer de datos UDP */

		//printf("Dibujamos\n");
//		SDL_RenderClear(g->renderer);
//		text_draw(&debug);
//		SDL_RenderPresent(g->renderer);

		SDL_Delay(SCREEN_REFRESH);
	}
	text_destroy(&debug);
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
				end = true;
				req_fill(&req,C_CONNECT_2,0);
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
		req_fill(&req,C_CONNECT_1,sizeof(req_connect_t));
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

	while(!end){
		/* Borramos la pantalla */
		SDL_RenderClear(g->renderer);
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

	void close_connection(void *g){
		game_t *gg = (game_t*)g;
		req_t req;
		void server_response_handle(res_t *res){
			game_set_status(gg,DISCONNECTED);
			printf("Cerramos la conexión\n");
			tcp_client_disconnect(gg->command_cli);
		}
		req_init(&req);
		req_fill(&req,C_DISCONNECT,0);
		printf("Enviamos desconectarnos\n");
		tcp_client_send(gg->command_cli,&req,&server_response_handle);
		printf("Enviamos desconectarnos. Recibimos respuesta\n");
		req_destroy(&req);
	}

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
		req_fill(&req,C_GAME_START,0);
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
						break;
					case SDLK_UP:
						menu_up(&menu);
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
		req_init(&req);
		req_fill(&req,C_GAME_RESUME,0);
		printf("Enviamos continuar\n");
		tcp_client_send(g->command_cli,&req,NULL);
		game_set_status(g,PLAYING);
		req_destroy(&req);
	}

	/* Terminar juego */
	void end_game(game_t *g){
		req_t req;
		req_init(&req);
		req_fill(&req,C_GAME_STOP,0);
		printf("Enviamos terminar\n");
		tcp_client_send(g->command_cli,&req,NULL);
		game_set_status(g,CONNECTED);
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

void game_free(game_t *g){
	SDL_Quit();
}
