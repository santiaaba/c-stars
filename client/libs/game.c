#include "game.h"

int game_init(game_t *game){
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("No fue posible iniciar el video.\n");
		printf("Error SDL: %s\n", SDL_GetError());
		return 1;
	}

	game->window = SDL_CreateWindow(
		"C-Start", 20, 20, SCREEN_WIDTH,
		SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if(game->window == NULL){
		printf("No fue posible crear la ventana.\n");
		printf("Error SDL: %s\n", SDL_GetError());
		return 1;
	}

	game->renderer = SDL_CreateRenderer(game->window, -1, 0);
	game->status = HELLO;
}

int game_check_connect(){
	/* Verifica que la conexion al server todavia
	   se encuentre activa */
}

void game_server_close(game_t *game){
	close(game->sockfd);
	game->status == DISCONNECT;
}

void *game_run(game_t *game){
	printf("Arrancamos GAME\n");
	while(game->status != END){
		switch(game->status){
			case HELLO:
				game_hello(game);
				break;
			case PLAY:
				game_play(game);
				break;
			case CONNECT:
				game_connect(game);
				break;
			case MAINMENU:
				game_main_menu(game);
				break;
			case PAUSE:
				game_pause(game);
				break;
		}
	}
}

void game_hello(game_t *game){
	/* Pantalla de presentacion. Se aguarda precionar
		cualquier tecla */
	SDL_Event event;
	SDL_Rect bg_frame, bg_dest;
	SDL_Texture *bg_texture;

	bg_frame.x = 0;
	bg_frame.y = 0;
	bg_frame.h = SCREEN_HEIGHT;
	bg_frame.w = SCREEN_WIDTH;

	bg_dest.x = 0;
	bg_dest.y = 0;
	bg_dest.h = SCREEN_HEIGHT;
	bg_dest.w = SCREEN_WIDTH;

	bg_texture = IMG_LoadTexture(game->renderer, "img/hello_bg.jpg");

	printf("Arrancamos HELLO\n");
	while(game->status == HELLO){
		/* Si presionamos cualquier tecla, pasamos la pantalla */
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_KEYDOWN)
				game->status = CONNECT;
		}
		/* Borramos la pantalla */
		SDL_RenderClear(game->renderer);
		/* Pintamos el background */
		SDL_RenderCopy(game->renderer, bg_texture, &bg_frame, &bg_dest);
		/* Le pedimos al screen que renderise */
		SDL_RenderPresent(game->renderer);
		SDL_Delay(SCREEN_REFRESH);

		/* Movemos el background */
		if(bg_frame.x < 200){
			bg_frame.x++;
		}
	}
	printf("Finalizo HELLO\n");
}

void game_play(game_t *game){
	/* En este modo, el juego esta corriendo. Se deben
		capturar los eventos del teclado y enviar los mismos
		al servidor del juego */

	SDL_Event event;

	while(game->status == PLAY){
		/* Capturamos los eventos del teclado */
		while(SDL_PollEvent(&event)){
			switch(event.key.keysym.sym){
				case SDLK_LEFT:
					break;
				case SDLK_RIGHT:
					break;
				case SDLK_UP:
					break;
				case SDLK_DOWN:
					break;
				case SDLK_BACKSPACE:
					break;
				case SDLK_ESCAPE:
					break;
			}
		}
		SDL_Delay(SCREEN_REFRESH);
	}
}

void game_connect(game_t *game){
	/* En este modo, el juego presenta una pantalla
		para ingresar la ip del servidor e intentar
		conectar al mismo. No se cambia de estado si
		no se logra la conexión */

	input_t ip_server;
	int key = 0;
	SDL_Event event;
	int key_press = 0;

	input_init(&ip_server,100,100,1,game->renderer);

	while(game->status == CONNECT){
		/* Capturamos los eventos del teclado */
		while(SDL_PollEvent(&event)){
			/* Solo aceptamos los numeros, el punto y el backspace */
			//if(event.type == SDL_KEYDOWN && !key_press){
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
					game->status = HELLO;
				if (key == SDLK_RETURN)
					printf("IMPLEMENTAR CONECTAR\n");
					/* CONECTAR */
					game_server_connect(game,input_value(&ip_server));
			} else {
				if(event.type == SDL_KEYUP)
					key_press = 0;
			}
		}
		/* Borramos la pantalla */
		SDL_RenderClear(game->renderer);
		/* render del input */
		input_draw(&ip_server,game->renderer);
		/* Render pantalla */
		SDL_RenderPresent(game->renderer);
		SDL_Delay(SCREEN_REFRESH);
	}
}

void game_main_menu(game_t *game){
	/* Loop cuando se esta presentando el menu
		principal. Las opciones del menu son:
		- Jugar
		- Creditos
		- Desconectar
		- Salir
	*/

	button_t buttons[4];			// Array de botones
	int button = 0;				// Indice del array anterior
	int i;
	int key;
	SDL_Event event;

	while(game->status == MAINMENU){
		while(SDL_PollEvent(&event)){
			/* Solo aceptamos las flechas arriba, abajo y enter */
			key = event.key.keysym.sym;
			if (key == SDLK_UP && button < 3 )
				button++;
			if (key == SDLK_DOWN && button > 0)
				button--;
			if (key == SDLK_RETURN)
				printf("IMPLEMENTAR BOTON\n");
				/* ejecutar button */
		}
		SDL_RenderClear(game->renderer);
		for(i=0; i<4; i++){
			if(i==button)
				button_focus(&(buttons[i]));
			else
				button_exit(&(buttons[i]));
			button_draw(&(buttons[i]),game->renderer);
		}
		SDL_Delay(SCREEN_REFRESH);
	}
}

void game_pause(game_t *game){
	/* Loop cuando se esta presentando la pausa */
	while(game->status == PAUSE){
		printf("IMPLEMENTAR\n");
	}
}

void game_free(game_t *game){
	SDL_Quit();
}
