#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <stdio.h>
#include <SDL2/SDL.h>

#define BACKGROUND_SPEED 1

#define BG_STATIC 0
#define BG_DINAMIC 1

typedef struct{
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	SDL_Rect cut;
	SDL_Rect destiny;
	int mod;
} background_t;

void background_init(background_t *bg, SDL_Renderer *renderer,
							SDL_Texture *texture, int mod);
//void background_begin(background_t *bg);
void background_draw(background_t *bg);

#endif
