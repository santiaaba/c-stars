#ifndef ENTITY_H
#define ENTITY_H
/* Representa un elemento a dibujar en pantalla.
   Una nave, un disparo, etc */

#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>

typedef struct t_entity{
	SDL_Rect frame;
	SDL_Rect position;
	SDL_Texture *texture;
} entity_t;

void entity_init(entity_t *entity, SDL_Texture *texture, uint16_t type);
void entity_set_position(entity_t *entity, int32_t x, int32_t y);
void entity_set_frame(entity_t *entity,uint16_t row, uint16_t col);

void entity_draw(entity_t *entity, SDL_Renderer *renderer);

void entity_copy(entity_t *dest, entity_t *src);
void entity_destroy(entity_t **entity);
#endif
