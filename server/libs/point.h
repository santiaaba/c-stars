#ifndef POINT_H
#define POINT_H

#include <stdlib.h>
#include <stdint.h>
#include "vector.h"

typedef struct Point {
	int32_t x;
	int32_t y;
} point_t;

void point_init(point_t *point);
void point_set(point_t *point, int32_t x, int32_t y);
void point_add_vector(point_t *point, vector_t *vector);
int32_t point_get_x(point_t *point);
int32_t point_get_y(point_t *point);
#endif
