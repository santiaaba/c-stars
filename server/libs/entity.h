#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include "clockgame.h"
#include "vector.h"
#include "border.h"
#include "point.h"
#include "animation.h"

typedef struct entity {
	point_t position;
	void *attributes;
	int state;
	border_t border;
	bool tangible;
	vector_t vector;
	animation_t animation;
} entity_t;

void entity_init(entity_t *e, void *attributes, void *attribute_destroy);
void entity_draw(entity_t *e);
bool entity_colision(entity_t *e, entity_t *e2);
void entity_do(entity_t *e);
void entity_destroy(entity_t **e);

#endif

