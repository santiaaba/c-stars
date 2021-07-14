/* Input de texto */

#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

typedef struct {
	SDL_Rect dest;
	SDL_Rect frame;
	SDL_Texture *off;
	SDL_Texture *on;
	int focus;
} button_t;

void button_init(button_t *button,
							char *file_off,
							char *file_on,
							int x,
							int y,
							int focus,
							SDL_Renderer *renderer);

void button_draw(button_t *button, SDL_Renderer *renderer);

void button_focus(button_t *button);

void button_exit(button_t *button);

#endif
