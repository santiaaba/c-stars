#include "point.h"

point_t *point_init(uint32_t x, uint32_t y){
	point_t *point;
	point = (point_t*)malloc(sizeof(point_t));
	return point;
}

void point_set(point_t *point, uint32_t x, uint32_t y){
}
uint32_t point_get_x(point_t *point){
	return point->x;
}
uint32_t point_get_y(point_t *point){
	return point->y;
}

