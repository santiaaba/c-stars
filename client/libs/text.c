/* Texto plano*/

#include "text.h"

void text_init(text_t *text,
							int x,
							int y,
							SDL_Renderer *renderer){

	text->dest.x = x;
	text->dest.y = y;
	text->dest.w = 100;		/* Ancho fijo al input */
	text->dest.h = 100;	 	/* Alto fijo al input */

	text->renderer = renderer;
	text->font = TTF_OpenFont("Sans.ttf", 24);
	text->color.r = 220;
	text->color.g = 220;
	text->color.b = 220;
	text->message = NULL;
}

void text_draw(text_t *text){
	
	if(text->message != NULL)
		SDL_RenderCopy(text->renderer, text->message, NULL, &(text->dest));
}

void text_set(text_t *text, char *value){

	SDL_Surface* surface;

	surface = TTF_RenderText_Solid(text->font, value, text->color); 
	text->message = SDL_CreateTextureFromSurface(text->renderer, surface);
}
