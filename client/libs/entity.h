#ifndef ENTITY_H
#define ENTITY_H
/* Representa un elemento a dibujar en pantalla.
   Una nave, un disparo, etc */

#include <stdio.h>
#include <stdint.h>

typedef struct t_entity{
	SDL_Surface *sprite;
	SDL_Rect frame;	  /* determina dentro del sprite la parte a dibujar */
	SDL_Rect position;  /* Solo nos interesan las coordenadas */
	uint16_t type;
} entity_t;

void entity_init(entity_t *entity, SDL_Surface *sprite, uint16_t type);
void entity_set_position(entity_t *entity, int32_t x, int32_t y);
void entity_set_frame(entity_t *entity,uint16_t row, uint16_t col);
void entity_draw(entity_t *entity, SDL_Surface *screen);
void entity_destroy(entity_t **entity);
#endif
