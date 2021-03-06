/* Texto plano*/

#include "text.h"

void text_init(text_t *text, int x, int y, int size,
					SDL_Renderer *renderer){

	text->dest.x = x;
	text->dest.y = y;
	text->renderer = renderer;
	text->size = size;
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
	TTF_Font *font;

	font = TTF_OpenFont(FONT, text->size);
	if(!font)
		printf("TTF_OpenFont: %s\n", TTF_GetError());	

	surface = TTF_RenderText_Solid(font, value, text->color); 
	text->message = SDL_CreateTextureFromSurface(text->renderer, surface);
	SDL_FreeSurface(surface);
	SDL_QueryTexture(text->message, NULL,NULL, &(text->dest.w), &(text->dest.h));

	TTF_CloseFont(font);
}

void text_destroy(text_t *text){
	SDL_DestroyTexture(text->message);
}
