#include "point.h"

void point_init(point_t *point){
	point->x = 0;
	point->y = 0;
}

void point_set(point_t *point, int32_t x, int32_t y){
	point->x = x;
	point->y = y;
}

void point_add_vector(point_t *point, vector_t *vector){
	point->x += vector_x(vector);
	point->y += vector_y(vector);
}

int32_t point_get_x(point_t *point){
	return point->x;
}

int32_t point_get_y(point_t *point){
	return point->y;
}

void point_copy(point_t *point, point_t *other){
	point->x = other->x;
	point->y = other->y;
}
