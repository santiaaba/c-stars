/* Texto plano */

#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <string.h>
#include <SDL2/SDL_ttf.h>

#define TEXT_VALUE_LIMIT 100 
#define FONT "fonts/turret-road.extrabold.ttf"

typedef struct {
	SDL_Rect dest;
	TTF_Font *font;
	SDL_Color color;
	SDL_Texture *message;
	SDL_Renderer *renderer;
} text_t;

void text_init(text_t *text, int x, int y, int size, SDL_Renderer *renderer);

void text_draw(text_t *text);

void text_set(text_t *text, char *value);

#endif
