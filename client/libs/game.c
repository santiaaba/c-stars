#include "game.h"

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
	g->udp = 20000;	// Lo fijamos
	if(sem_init(&(g->sem_status),0,1) == -1){
		fprintf(stderr, "game_init() failed: %s\n", strerror(errno));
	}
	printf("Paso\n");
	g->command_cli = (tcp_client_t *)malloc(sizeof(tcp_client_t));
	printf("Paso2\n");
}

int game_check_connect(){
	/* Verifica que la conexion al server todavia
	   se encuentre activa */
}

void static game_set_status(game_t *g, int status){
	sem_wait(&(g->sem_status));
		g->status = status;
	sem_post(&(g->sem_status));
}

void *game_run(game_t *g){
	printf("Arrancamos GAME\n");
	while(g->status != END){
		switch(g->status){
			case HELLO:
				game_hello(g);
				break;
			case PLAYING:
				game_play(g);
				break;
			case CONNECT:
				game_connect(g);
				break;
			case MAINMENU:
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
				g->status = CONNECT;
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

	SDL_Event event;
	SDL_Rect frame;
	SDL_Rect position;
	int i,j;
	char *buffer_req;
	int buffer_req_size;
	data_t *data;
	req_t req;

	req_init(&req);
	req_fill(&req,C_KEY_PRESS,4);
	req.body = (req_kp_t*)malloc(sizeof(req_kp_t));
	g->screen_frame = 0;
	while(g->status == PLAYING){
		/* Capturamos los eventos del teclado y los colocamos en el
			buffer compartido con el hilo del cliente de comandos TCP */
		printf("Capturamos y enviamos eventos\n");
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_KEYUP &&
				event.key.keysym.sym == SDLK_ESCAPE){
					game_set_status(g,MAINMENU);
					break;
			}
			if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) &&
				(event.key.keysym.sym == SDLK_LEFT ||
				 event.key.keysym.sym == SDLK_RIGHT ||
			 	 event.key.keysym.sym == SDLK_UP ||
				 event.key.keysym.sym == SDLK_DOWN ||
				 event.key.keysym.sym == SDLK_BACKSPACE )){

				((req_kp_t*)(req.body))->key = event.key.keysym.sym;
				((req_kp_t*)(req.body))->action = event.type;
				printf("Enviamos evento-->\n");
				tcp_client_send(g->command_cli,&req,NULL);
			}
		}

		/* Dibutajos la pantalla si es que tenemos informacion
			en el buffer de datos UDP */

		printf("Dibujamos\n");
/*
		sem_wait(g->sem_render);
		for(i=0;i<g->buffer_render_size;i++){
			data = &(g->buffer_render[i]);

			/* Si la data corresponde a un frame nuevo entonces
				dibujamos la pantalla. */
/*
			if(data->header.frame != g->screen_frame && g->screen_frame != 0){
				g->screen_frame = data->header.frame;
				SDL_RenderPresent(g->renderer);
				SDL_RenderClear(g->renderer);
			}
			/* armamos el dato */
/*
			for(j=0;j<data->header.size;j++){
				position.x = data->body[j].pos_x;
				position.y = data->body[j].pos_y;
				position.w = g->entities[data->body[j].entity_class].w;
				position.h = g->entities[data->body[j].entity_class].h;

				frame.w = position.w;
				frame.h = position.h;
				frame.x = frame.h * data->body[j].sprite;
				frame.y = frame.w * data->body[j].frame;
				SDL_RenderCopy(
					g->renderer,
					g->entities[data->body[j].entity_class].texture,
					&frame,
					&position);
			}
		}
		g->buffer_render_size = 0;
		sem_post(g->sem_render);
*/
		SDL_Delay(SCREEN_REFRESH);
	}
}

void game_connect(game_t *g){
	/* En este modo, el juego presenta una pantalla
		para ingresar la ip del servidor e intentar
		conectar al mismo. No se cambia de estado si
		no se logra la conexión. Presionando ESC
	   se regresa a la pantalla hello */

	input_t ip_server;
	text_t error;
	pthread_t th;		// Para indicarle al server iniciar el juego
	int key = 0;
	SDL_Event event;
	int key_press = 0;
	int wait = false;
	char *srv_ip;
	char message[200];
	char borrar[1000];

	void *try_to_connect(void *g){
		game_t *gg = (game_t*)g;
		req_t req;
		char *buffer_req = NULL;
		int buffer_req_size = 0;

		void server_response_handle(res_t *res){
			printf("Manejando la respuesta del server\n");
			if(res->header.resp == RES_OK){
				game_set_status(gg,MAINMENU);
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

	input_init(&ip_server,100,100,1,g->renderer);
	text_init(&error,200,200,25,g->renderer);

	while(g->status == CONNECT){
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
						g->status = HELLO;
					if (key == SDLK_RETURN){
						wait = true;
						srv_ip = input_get_value(&ip_server);
						printf("Creando el hilo\n");
						text_set(&error,"Tratando de conectar");
						pthread_create(&th,NULL,&try_to_connect,g);
						/* CONECTAR */
					}
				} else {
					if(event.type == SDL_KEYUP)
						key_press = 0;
				}
			}
			/* render del input */
			input_draw(&ip_server);
			text_draw(&error);
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
		- Salir
	*/
	const int cant_buttons = 3;

	button_t buttons[cant_buttons];		// Array de 3 botones
	int button = 0;							// Boton en foco
	int i;
	int key;
	SDL_Event event;
	bool pusshed;
	pthread_t th;
	bool showButtons;
	text_t message;

	void on_focus(button_t *buttons, int b, int size){
		for(i=0; i<size; i++){
			if(i==b)
				button_focus(&(buttons[i]));
			else
				button_exit(&(buttons[i]));
		}
	}

	void close_connection(void *g){
		game_t *gg = (game_t*)g;
		req_t req;
		void server_response_handle(res_t *res){
			game_set_status(gg,HELLO);
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
				text_set(&message,"Error");
				showButtons = true;
			}
		}

		/* Enviar mensaje para iniciar el juego */
		req_init(&req);
		req_fill(&req,C_GAME_START,0);
		tcp_client_send(gg->command_cli,&req,&server_response_handle);
		req_destroy(&req);
	}

	button_init(&(buttons[0]),100,50,400,50,1,g->renderer);
	button_bg_color(&(buttons[0]),100,10,255,100,100,100);
	button_font_color(&(buttons[0]),0,0,0,200,200,0);
	button_border_color(&(buttons[0]),0,0,0,200,200,0);
	button_text(&(buttons[0]),"Jugar");

	button_init(&(buttons[1]),100,120,400,50,0,g->renderer);
	button_bg_color(&(buttons[1]),100,10,255,100,100,100);
	button_font_color(&(buttons[1]),0,0,0,200,200,0);
	button_border_color(&(buttons[1]),0,0,0,200,200,0);
	button_text(&(buttons[1]),"Creditos");

	button_init(&(buttons[2]),100,190,400,50,0,g->renderer);
	button_bg_color(&(buttons[2]),100,10,255,100,100,100);
	button_font_color(&(buttons[2]),0,0,0,200,200,0);
	button_border_color(&(buttons[2]),0,0,0,200,200,0);
	button_text(&(buttons[2]),"Desconectar");

	printf("--- MAIN-MENU ---\n");
	pusshed = false;
	showButtons = true;
	text_init(&message,200,200,25,g->renderer);
	while(g->status == MAINMENU){
		while(SDL_PollEvent(&event)){
			if(!showButtons)
				break;
			if(!pusshed && event.type == SDL_KEYDOWN){
				pusshed=true;
				/* Solo aceptamos las flechas arriba, abajo y enter */
				key = event.key.keysym.sym;
				if (key == SDLK_DOWN && button < cant_buttons - 1 ){
					printf("Bajamos en el menu\n");
					button++;
					on_focus(buttons,button,cant_buttons);
				}
				if (key == SDLK_UP && button > 0){
					printf("Subimos en el menu\n");
					button--;
					on_focus(buttons,button,cant_buttons);
				}
				if (key == SDLK_RETURN)
					switch(button){
						case 0:
							/* Impedimos los eventos */
							showButtons=false;
							text_set(&message,"Iniciando Juego");
							pthread_create(&th,NULL,&pre_start_game,g);
							break;
						case 1:
							break;
						case 2:
							/* FALTA CERRAR EL SERVIDOR UDP */
							close_connection(g);
					}
					printf("IMPLEMENTAR BOTON\n");
					/* ejecutar boton seleccionado */
			}
			if(pusshed && event.type == SDL_KEYUP)
				pusshed=false;
		}
		SDL_RenderClear(g->renderer);
		if(showButtons)
			for(i=0; i<cant_buttons; i++)
				button_draw(&(buttons[i]));
		text_draw(&message);
		SDL_RenderPresent(g->renderer);
		SDL_Delay(SCREEN_REFRESH);
	}
}

void game_pause(game_t *g){
	/* Loop cuando se esta presentando la pausa */

	const int cant_buttons = 2;

	button_t buttons[cant_buttons];		// Array de 3 botones
	int button = 0;							// Boton en foco

	/* Continuar juego */
	void resume_game(void *g){
ACA ME QUEDE
/*
		game_t *gg = (game_t*)g;
		req_t req;
		void server_response_handle(res_t *res){
			game_set_status(gg,HELLO);
			printf("Cerramos la conexión\n");
			tcp_client_disconnect(gg->command_cli);
		}
		req_init(&req);
		req_fill(&req,C_DISCONNECT,0);
		printf("Enviamos desconectarnos\n");
		tcp_client_send(gg->command_cli,&req,&server_response_handle);
		printf("Enviamos desconectarnos. Recibimos respuesta\n");
		req_destroy(&req);
*/
	}

	/* Terminar juego */
	void end_game(void *g){
	}

	button_init(&(buttons[0]),100,50,400,50,1,g->renderer);
	button_bg_color(&(buttons[0]),100,10,255,100,100,100);
	button_font_color(&(buttons[0]),0,0,0,200,200,0);
	button_border_color(&(buttons[0]),0,0,0,200,200,0);
	button_text(&(buttons[0]),"Continuar");

	button_init(&(buttons[1]),100,120,400,50,1,g->renderer);
	button_bg_color(&(buttons[1]),100,10,255,100,100,100);
	button_font_color(&(buttons[1]),0,0,0,200,200,0);
	button_border_color(&(buttons[1]),0,0,0,200,200,0);
	button_text(&(buttons[1]),"Terminar");

	pusshed = false;
	while(g->status == PAUSE){
		while(SDL_PollEvent(&event)){
			if(!showButtons)
				break;
			if(!pusshed && event.type == SDL_KEYDOWN){
				pusshed=true;
				/* Solo aceptamos las flechas arriba, abajo y enter */
				key = event.key.keysym.sym;
				if (key == SDLK_DOWN && button < cant_buttons - 1 ){
					printf("Bajamos en el menu\n");
					button++;
					on_focus(buttons,button,cant_buttons);
				}
				if (key == SDLK_UP && button > 0){
					printf("Subimos en el menu\n");
					button--;
					on_focus(buttons,button,cant_buttons);
				}
				if (key == SDLK_RETURN)
					switch(button){
						case 0:
							/* Continuar el juego */
							break;
						case 1:
							/* Finalizar el juego */
							break;
					}
			}
			if(pusshed && event.type == SDL_KEYUP)
				pusshed=false;
		}
		SDL_RenderClear(g->renderer);
		if(showButtons)
			for(i=0; i<cant_buttons; i++)
				button_draw(&(buttons[i]));
		SDL_RenderPresent(g->renderer);
		SDL_Delay(SCREEN_REFRESH);
	}
}

void game_free(game_t *g){
	SDL_Quit();
}
