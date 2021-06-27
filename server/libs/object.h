#ifndef OBJECT_H
#define OBJECT_H

#include <stdlib.h>
#include <stdint.h>
#include "vector.h"
#include "point.h"

typedef enum { SHUT, ROCK, WEAPON } object_type_t;

typedef struct t_object {
	point_t point;
	uint32_t damage;
	border_t *border;
	vector_t *vector;
	object_type_t type;
} object_t;

void object_init(object_t *object);
void object_set_position(object_t *object, point_t p);
void object_add_vector(object_t *object, vector_t *vector);
int32_t object_get_x(object_t *object);
int32_t object_get_y(object_t *object);
void object_set_damage(object_t *object, uint32_t damage);
uint32_t object_get_damage(object_t *object);
#endif
