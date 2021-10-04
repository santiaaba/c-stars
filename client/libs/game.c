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
	printf("ACA");
	sem_init(&(g->sem_status),0,1);
	g->command_cli = (tcp_client_t *)malloc(sizeof(tcp_client_t));
	printf("ACA paso");
}

int game_check_connect(){
	/* Verifica que la conexion al server todavia
	   se encuentre activa */
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
	/* En este modo, el juego esta corriendo. 
		Se deben dibujar todos los frames en base
		a lo que se encuentra en el buffer. Se deben
		capturar los eventos del teclado y enviar los mismos
		al servidor del juego */

	SDL_Event event;
	SDL_Rect frame;
	SDL_Rect position;
	int i,j;
	char *buffer_req;
	int buffer_req_size;
	data_t *data;
	req_t req;

	req.body = (req_kp_t*)malloc(sizeof(req_kp_t));
	g->screen_frame = 0;
	while(g->status == PLAYING){
		/* Capturamos los eventos del teclado y los colocamos en el
			buffer compartido con el hilo del cliente de comandos TCP */
		while(SDL_PollEvent(&event)){
			if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) &&
				(event.key.keysym.sym == SDLK_LEFT ||
				 event.key.keysym.sym == SDLK_RIGHT ||
			 	 event.key.keysym.sym == SDLK_UP ||
				 event.key.keysym.sym == SDLK_DOWN ||
				 event.key.keysym.sym == SDLK_BACKSPACE ||
				 event.key.keysym.sym == SDLK_ESCAPE)){

				((req_kp_t*)(req.body))->key = event.key.keysym.sym;
				((req_kp_t*)(req.body))->action = event.type;
				tcp_client_send(g->command_cli,&req,NULL);
			}
		}

		/* Dibutajos la pantalla si es que tenemos informacion
			en el buffer de datos UDP */

		sem_wait(g->sem_render);
		for(i=0;i<g->buffer_render_size;i++){
			data = &(g->buffer_render[i]);

			/* Si la data corresponde a un frame nuevo entonces
				dibujamos la pantalla. */
			if(data->header.frame != g->screen_frame && g->screen_frame != 0){
				g->screen_frame = data->header.frame;
				SDL_RenderPresent(g->renderer);
				SDL_RenderClear(g->renderer);
			}
			/* armamos el dato */
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

		SDL_Delay(SCREEN_REFRESH);
	}
}

void static game_set_status(game_t *g, int status){
	sem_wait(&(g->sem_status));
		g->status = status;
	sem_post(&(g->sem_status));
}

void game_connect(game_t *g){
	/* En este modo, el juego presenta una pantalla
		para ingresar la ip del servidor e intentar
		conectar al mismo. No se cambia de estado si
		no se logra la conexión. Presionando ESC
	   se regresa a la pantalla hello */

	input_t ip_server;
	text_t error;
	int key = 0;
	SDL_Event event;
	int key_press = 0;
	int wait = false;
	char *srv_ip;
	pthread_t th;
	char message[200];
	char borrar[1000];

	void *try_to_connect(void *g){

		game_t *gg = (game_t*)g;
		req_t req;
		char *buffer_req = NULL;
		int buffer_req_size = 0;

		void server_response_handle(res_t *res){
			printf("Manejando la respuesta del cliente\n");
			if(res->header.resp == RES_OK){
				game_set_status(gg,MAINMENU);
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
			}
			wait = false;
		}

		printf("Creando el fileDescriptor %s:%i\n",srv_ip,SRV_PORT);
		if(!tcp_client_init(gg->command_cli,srv_ip,SRV_PORT)){
			text_set(&error,"Error fatal al crear el socket");
			printf("Error al crear el FD\n");
			wait = false;
			return NULL;
		}
		
		printf("Conectando contra el servidor\n");
		if(!tcp_client_connect(gg->command_cli)){
			text_set(&error,"Server no responde");
			printf("Error server no responde\n");
			wait = false;
			return NULL;
		}
		printf("Servidor conectado. Esperamos 10 Segundos\n");
		printf("Enviando udp y version\n");
		req.body = (req_connect_t*)malloc(sizeof(req_connect_t));
		req_init(&req,C_CONNECT_1,sizeof(req_connect_t));

		((req_connect_t*)(req.body))->udp = gg->udp;
		((req_connect_t*)(req.body))->version = EAEAPP_VERSION;

		tcp_client_send(gg->command_cli,&req,&server_response_handle);
		printf("paso envio a servidor\n");
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

	button_t buttons[4];			// Array de botones
	int button = 0;				// Boton en foco
	int i;
	int key;
	SDL_Event event;

	void on_focus(button_t *buttons; int b, int size){
		ACA ME QUEDE. PASAR ARRAY POR REFERENCIA
		for(i=0; i<size; i++){
			if(i==b)
				button_focus(&(buttons[i]));
			else
				button_exit(&(buttons[i]));
		}
	}

	while(g->status == MAINMENU){
		while(SDL_PollEvent(&event)){
			/* Solo aceptamos las flechas arriba, abajo y enter */
			key = event.key.keysym.sym;
			if (key == SDLK_UP && button < 3 ){
				button++;
				on_focus(buttons,button,4);
			}
			if (key == SDLK_DOWN && button > 0){
				button--;
				on_focus(buttons,button,4);
			}
			if (key == SDLK_RETURN)
				printf("IMPLEMENTAR BOTON\n");
				/* ejecutar boton seleccionado */
		}
		SDL_RenderClear(g->renderer);
		for(i=0; i<4; i++)
			button_draw(&(buttons[i]),g->renderer);
		SDL_RenderPresent(g->renderer);
		SDL_Delay(SCREEN_REFRESH);
	}
}

void game_pause(game_t *g){
	/* Loop cuando se esta presentando la pausa */
	while(g->status == PAUSE){
		printf("IMPLEMENTAR\n");
		SDL_Delay(SCREEN_REFRESH);
	}
}

void game_free(game_t *g){
	SDL_Quit();
}
