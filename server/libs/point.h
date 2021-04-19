#include <stdlib.h>
#include <stdint.h>

#ifndef POINT_H
#define POINT_H
typedef struct Point {
	uint32_t x;
	uint32_t y;
} point_t;

point_t *point_init(uint32_t x, uint32_t y);
void point_set(point_t *point, uint32_t x, uint32_t y);
uint32_t point_get_x(point_t *point);
uint32_t point_get_y(point_t *point);
#endif
