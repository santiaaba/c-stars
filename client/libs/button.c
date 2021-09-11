#include "button.h"

void button_init(button_t *button,
							char *file_off,
							char *file_on,
							int x,
							int y,
							int focus,
							SDL_Renderer *renderer){

	button->off = IMG_LoadTexture(renderer, file_off);
	button->on = IMG_LoadTexture(renderer, file_on);

	button->focus = focus;

	button->frame.x = 0;
	button->frame.y = 0;
	button->frame.w = 200;	/* Ancho fijo al boton */
	button->frame.h = 50;		/* Alto fijo al boton */

	button->dest.x =	x;
	button->dest.y = y;
	button->dest.w = 200;		/* Ancho fijo al boton */
	button->dest.h = 50;		/* Alto fijo al boton */
}

void button_draw(button_t *button, SDL_Renderer *renderer){
	if(button->focus){
		SDL_RenderCopy(renderer,button->on,&(button->frame),&(button->dest));
	} else {
		SDL_RenderCopy(renderer,button->off,&(button->frame),&(button->dest));
	}
}

void button_focus(button_t *button){
	button->focus = 1;
}

void button_exit(button_t *button){
	button->focus = 0;
}
