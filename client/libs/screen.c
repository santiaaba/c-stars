int screen_init(screen_t *screen){
	screen->entities = 0;

	if(SDL_init(SDL_INIT_VIDEO) < 0){
		printf("No fue posible iniciar el video. ");
		printf("Error SDL: %s\n", SDL_GetError());
		return 1
	}

	screen->window = SDL_CreateWindow(
		"C-Start", 20, 20, SCREEN_WIDTH,
		SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if(screen->windows == NULL){
		printf("No fue posible crear la ventana. ");
		printf("Error SDL: %s\n", SDL_GetError());
		return 1
	}
	
	screen->playGround = SQL_GetWindowSurface(screen->window);
}

static void screen_draw(screen_t *screen){
	uint16_t i;
	for(i=0; i < (screen->entities); i++)
		entity_draw(screen->buffer[i],screen->play_groud)
	screen->entities = 0;
}

void screen_add_data(screen_t *screen, void udpdata){
	/* Agrega entidades segun los datos recibidos */
	/* Si el frame se ha incrementado entonces pasa a la
		pantalla el render de todas las entidades */
	uint16_t i;
	if(frame > screen->frame){
		screen->frame = frame;
		screen_draw(screen);
	}
	/* Generamos entidades segun los datos */
	while(/* ALGO */){
		screen->entities++;
		i = screen->entities
		screen->buffer[i].x
		screen->buffer[i].y
	}
}

void screen_destroy(screent_t **screen){
	lista_clean(*screen->lista);
	SDL_DestroyWindow(screen->window);
   SDL_Quit();
}
