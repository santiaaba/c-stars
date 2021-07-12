#include "screen.h"

int screen_init(screen_t *screen){
	screen->entities = 0;

	if(SDL_init(SDL_INIT_VIDEO) < 0){
		printf("No fue posible iniciar el video. ");
		printf("Error SDL: %s\n", SDL_GetError());
		return 1;
	}

	screen->window = SDL_CreateWindow(
		"C-Start", 20, 20, SCREEN_WIDTH,
		SCREEN_HEIGHT, SDL_WINDOW_SHOW);

	if(screen->window == NULL){
		printf("No fue posible crear la ventana. ");
		printf("Error SDL: %s\n", SDL_GetError());
		return 1;
	}
	
	screen->play_ground = SDL_CreateRenderer(screen->window, -1, 0);
}

static void screen_draw(screen_t *screen){
	uint16_t i;
	SDL_RenderClear(renderer);
	for(i=0; i < (screen->entities); i++)
		entity_draw(screen->buffer[i],screen->play_groud);
	screen->entities = 0;
	SDL_RenderPresent(screen->play_ground);
}

void screen_add_data(screen_t *screen, char *buffer, uint16_t size){
	/* Agrega entidades segun los datos recibidos */
	/* Si el frame se ha incrementado entonces pasa a la
		pantalla el render de todas las entidades */

	https://www.cnblogs.com/zi-xing/p/6821543.html

	uint16_t aux;

	/* Los primeros 2bytes representan representan el frame */
	frame = memcopy(frame,buffer[0],2);
	if(frame > screen->frame){
		screen->frame = frame;
		screen_draw(screen);
	}
	/* Generamos entidades segun los datos */
	aux = 2;
	while( aux < size){
		entity = memcopy(frame,buffer[aux],SIZE_ENTITY)
		screen->entities++;
		i = screen->entities;
		screen->buffer[i].x;
		screen->buffer[i].y;
		aux += SIZE_ENTITY;
	}
}

void screen_destroy(screent_t **screen){
	lista_clean(*screen->lista);
	SDL_DestroyWindow(screen->window);
   SDL_Quit();
}
