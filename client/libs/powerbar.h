#ifndef POWERBAR_H
#define POWERBAR_H

#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define CANT_BARS		20

typedef struct {
	SDL_Texture *texture;
	SDL_Renderer *renderer;
	SDL_Rect cut;
	SDL_Rect dest;
	int x;
	int y;
	int max;
	int power;
	int cant;
} powerbar_t;

void powerbar_init(powerbar_t *bar, SDL_Renderer *renderer);
void powerbar_set_max(powerbar_t *bar, int max);
void powerbar_set_power(powerbar_t *bar, int power);
void powerbar_set_position(powerbar_t *bar, int x, int y);
void powerbar_draw(powerbar_t *bar);
void powerbar_destroy(powerbar_t *bar);

#endif
